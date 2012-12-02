using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;


namespace ptsi_web
{
    abstract public class DataReader
    {
        public enum DataType { Oxygen, Pulse, ECG };
        public enum DataSource { Server, File, Database };

        protected UInt64 PESEL;
        protected byte dataType;

        private List<byte> dataBuffer = new List<byte>();
        private int index;

        protected static Log drLog = new Log("datareader.txt");

        public DataReader(UInt64 PESEL_, DataType dataType_)
        {
            PESEL = PESEL_;
            dataType = (byte)dataType_;
        }

        protected void Append(byte[] data)
        {
            dataBuffer.AddRange(data);
        }
        protected void Clear()
        {
            dataBuffer.RemoveRange(0, index);
        }
        //reads single byte to update dynamic info page
        public byte ReadByte()
        {
            byte result = dataBuffer[index];
            if(index < dataBuffer.Count-1) ++index;

            return result;
        }

        public byte[] ReadAll()
        {
            return dataBuffer.ToArray();
        }

        public byte[] ReadBytes(int num)
        {
            byte[] result = dataBuffer.GetRange(index - num + 1, num).ToArray();
            if (index + num < dataBuffer.Count - 2)
            {
                index += num;
            }
            else { index = dataBuffer.Count - 1; }

            return result;
        }

        //gets data from data source (which is defined in the child class)
        abstract public void GetData();

    }

    public class DataReaderServer : DataReader
    {
        //ServerConnection server = new ServerConnection("10.0.0.21", 40001);
        ServerConnection server = new ServerConnection();

        private static Random r = new Random();
        public byte READER_ID;

        public DataReaderServer(UInt64 PESEL_, DataType dataType_)
            : base(PESEL_, dataType_)
        {
            READER_ID = (byte)(r.Next() % 256);
            server.SendInit(PESEL, READER_ID, dataType);
        }

        override public void GetData()
        {
            server.RequestFileSize(PESEL, READER_ID, dataType);
            UInt32 fileSize = server.ReadFileSize();
            server.RequestFileData(PESEL, READER_ID, dataType, 0, fileSize);
            byte[] fileData = server.ReadFileData(fileSize);

            Append(fileData);

            //drLog.Write("", fileData);
        }
    }

    public class DataReaderFile : DataReader
    {
        private const string defaultFile = "E:/Studia/Semestr7/PTSI/L/git/ptsi_server/randomdata/data1.txt";
        private string filename;
        private long filePointer;

        public DataReaderFile(UInt64 PESEL_, DataType dataType_, string filename_ = defaultFile)
            : base(PESEL_, dataType_)
        {
            filename = filename_;
            filePointer = 0;
            
            
        }

        ~DataReaderFile()
        {
            
        }

        override public void GetData()
        {
            System.IO.FileStream file = new System.IO.FileStream(filename, System.IO.FileMode.Open);
            System.IO.BinaryReader reader = new System.IO.BinaryReader(file);

            reader.BaseStream.Seek(filePointer, System.IO.SeekOrigin.Begin);

            byte[] fileData = reader.ReadBytes(32);
            filePointer += 32;

            Append(fileData);
            reader.Close();
            file.Close();
        }
    }

    public class DataReaderDatabase : DataReader
    {
        private DatabaseConnection database = new DatabaseConnection();
        
        public DataReaderDatabase(UInt64 PESEL_, DataType dataType_)
            : base(PESEL_, dataType_)
        {


            
        }

        public override void GetData()
        {

            byte[] outData = database.ReadData(PESEL, (DataType)dataType);
            

            Append(outData);
        }

        public byte[] GetHistory(DataType type)
        {
            return database.ReadHistory(PESEL, type);
        }
    }

}