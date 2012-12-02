using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace ptsi_web
{

    public class DataViewer
    {
        static int LIST_SIZE = 256;

        private LinkedList<byte> dataList = new LinkedList<byte>();

        public DataViewer()
        {
            for (int i = 0; i < LIST_SIZE; ++i)
            {
                dataList.AddFirst(0);
            }
        }

        public void Add(byte b)
        {
            dataList.RemoveFirst();
            dataList.AddLast(b);
        }

        public string GetLast()
        {
            byte last = dataList.Last.Value;
            return last.ToString();
        }

        public byte[] GetBytes()
        {
            return dataList.ToArray();
        }

        override public string ToString()
        {
            string result = "";

            bool first = true;
            foreach (byte b in dataList)
            {
                if (first) first = false;
                else result += ",";
                result += b.ToString();
            }
            return result;
        }


    }
}