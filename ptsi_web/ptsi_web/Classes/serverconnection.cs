using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net.Sockets;

namespace ptsi_web
{
    public class ServerConnection
    {
        private static int count = 0;

        private static System.Net.Sockets.TcpClient clientSocket = new TcpClient();
        private static Log defaultLog = new Log("server.txt");

        private static string IPaddress;
        private static Int32 port;

        private int offset = 0;

        public ServerConnection()
        {
            ConfigParser p = Global.parser;

            IPaddress = p.GetValue("server", "IP");
            port = Convert.ToInt32(p.GetValue("server", "port"));

            ++count;
            if (!clientSocket.Connected)
            {
                InitConnection(IPaddress, port);
            }
        }

        ~ServerConnection()
        {
            --count;
            if (count <= 0) EndConnection();
        }

        private static void InitConnection(string IPaddress, Int32 port)
        {
            clientSocket.Connect(IPaddress, port);
            defaultLog.Write("Connected to " + IPaddress + ":" + port.ToString());
        }

        private static void EndConnection()
        {
            clientSocket.Close();
        }

        protected void JoinToPacket(ref byte[] destination, byte[] source, ref int index, int size)
        {
            for (int i = 0; i < size; ++i)
            {
                destination[index] = source[i];
                ++index;
            }
        }


        protected byte[] CreateInitPacket(UInt64 PESEL, byte readerID, byte type, UInt32 requestSize)
        {
            byte[] data = new byte[4 + 8 + 1 + 1 + 1 + 4];

            UInt32 lenght = (UInt32)data.Length - 4;

            byte[] dataLenght = BitConverter.GetBytes(lenght);
            byte[] dataPESEL = BitConverter.GetBytes(PESEL);
            byte[] dataReaderId = { readerID };
            byte[] dataMode = { 2 };
            byte[] dataType = { type };
            byte[] dataRequestSize = BitConverter.GetBytes(requestSize);



            int dataIndex = 0;

            JoinToPacket(ref data, dataLenght, ref dataIndex, 4);
            JoinToPacket(ref data, dataPESEL, ref dataIndex, 8);
            JoinToPacket(ref data, dataReaderId, ref dataIndex, 1);
            JoinToPacket(ref data, dataMode, ref dataIndex, 1);
            JoinToPacket(ref data, dataType, ref dataIndex, 1);
            JoinToPacket(ref data, dataRequestSize, ref dataIndex, 4);


            return data;
        }

        protected byte[] CreateGetLenghtPacket(UInt64 PESEL, byte readerID, byte type)
        {
            byte[] data = new byte[4 + 8 + 1 + 1 + 1];

            UInt32 lenght = (UInt32)data.Length - 4;

            byte[] dataLenght = BitConverter.GetBytes(lenght);
            byte[] dataPESEL = BitConverter.GetBytes(PESEL);
            byte[] dataReaderId = {readerID};
            byte[] dataMode = { 0 };
            byte[] dataType = { type };
            


            int dataIndex = 0;

            JoinToPacket(ref data, dataLenght, ref dataIndex, 4);
            JoinToPacket(ref data, dataPESEL, ref dataIndex, 8);
            JoinToPacket(ref data, dataReaderId, ref dataIndex, 1);
            JoinToPacket(ref data, dataMode, ref dataIndex, 1);
            JoinToPacket(ref data, dataType, ref dataIndex, 1);
            

            return data;
        }

        protected byte[] CreateReadPacket(UInt64 PESEL, byte readerId, byte type, UInt32 size)
        {

            byte[] data = new byte[4 + 8 + 1 + 1 + 1 + 4];

            UInt32 lenght = (UInt32)data.Length - 4;

            byte[] dataLenght = BitConverter.GetBytes(lenght);
            byte[] dataPESEL = BitConverter.GetBytes(PESEL);
            byte[] dataReaderId = { readerId };
            byte[] dataType = { type };
            byte[] dataMode = { 1 };
            byte[] dataSize = BitConverter.GetBytes(size);


            int dataIndex = 0;

            JoinToPacket(ref data, dataLenght, ref dataIndex, 4);
            JoinToPacket(ref data, dataPESEL, ref dataIndex, 8);
            JoinToPacket(ref data, dataReaderId, ref dataIndex, 1);
            JoinToPacket(ref data, dataMode, ref dataIndex, 1);
            JoinToPacket(ref data, dataType, ref dataIndex, 1);
            JoinToPacket(ref data, dataSize, ref dataIndex, 4);

            return data;
        }

        public void SendInit(UInt64 PESEL, byte readerId, byte dataType)
        {
            System.Net.Sockets.NetworkStream stream = clientSocket.GetStream();
            byte[] outInit = CreateInitPacket(PESEL, readerId, dataType, 32);
            defaultLog.Write("Sending init:", outInit);
            stream.Flush();
            stream.Write(outInit, 0, outInit.Length);
        }

        public void RequestFileSize(UInt64 PESEL, byte readerId, byte dataType)
        {
            System.Net.Sockets.NetworkStream stream = clientSocket.GetStream();
            byte[] outGetLenght = CreateGetLenghtPacket(PESEL, readerId, dataType);
            defaultLog.Write("Requesting file size:", outGetLenght);
            stream.Flush();
            stream.Write(outGetLenght, 0, outGetLenght.Length);
        }

        public UInt32 ReadFileSize()
        {
            System.Net.Sockets.NetworkStream stream = clientSocket.GetStream();
            defaultLog.Write("Reading file size:");
            byte[] inFileSize = new byte[4];
            stream.Flush();
            for (int i = 0; i < 4; ++i)
            {
                inFileSize[i] = (byte)stream.ReadByte();
            }

            UInt32 fileSize = BitConverter.ToUInt32(inFileSize, 0);

            defaultLog.Write("response: ", inFileSize);
            defaultLog.Write("filesize: " + fileSize.ToString());
            return fileSize;
        }

        public void RequestFileData(UInt64 PESEL, byte readerId, byte dataType, UInt32 offset, UInt32 fileSize)
        {
            System.Net.Sockets.NetworkStream stream = clientSocket.GetStream();
            byte[] outRead = CreateReadPacket(PESEL, readerId, dataType, fileSize);
            defaultLog.Write("Requesting file:", outRead);
            stream.Flush();
            stream.Write(outRead, 0, outRead.Length);
        }

        public byte[] ReadFileData(UInt32 fileSize = 0)
        {
            System.Net.Sockets.NetworkStream stream = clientSocket.GetStream();
            defaultLog.Write("Reading file:");
            byte[] inFileData = new byte[fileSize];

            stream.Flush();
            //if fileSize is 0 we read the size from the first 4 bytes
            if (fileSize == 0)
            {
                byte[] fileSizeData = new byte[4];
                for (int i = 0; i < 4; ++i)
                {
                    fileSizeData[i] = (byte)stream.ReadByte();
                }
                fileSize = BitConverter.ToUInt32(fileSizeData, 0);
            }
            //otherwise we just skip first 4 bytes
            else
            {
                for (int i = 0; i < 4; ++i) stream.ReadByte();
            }

            //read the actual data
            for (int i = 0; i < fileSize; ++i)
            {
                inFileData[i] = (byte)stream.ReadByte();
            }

            defaultLog.Write("file read: ", inFileData);
            return inFileData;
        }

        string StringFromFileData(byte[] fileData)
        {
            return System.Text.Encoding.UTF8.GetString(fileData);
        }


    }
}