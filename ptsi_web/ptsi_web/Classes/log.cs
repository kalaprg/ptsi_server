using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace ptsi_web
{
    public class Log
    {
        private string path;

        public Log() {}
        public Log(string path_) { path = Global.rootPath+"/"+path_; }

        void SetPath(string path_) {path = path_;}
        string GetPath() { return path; }

        public void Write(string s, byte[] data = null)
        {
            System.IO.StreamWriter f = new System.IO.StreamWriter(path, true);
            DateTime time = System.DateTime.Now;

            string txt = time.ToString();
            txt = txt + " " + s;

            if (data != null)
            {
                txt += " [";
                for (int i = 0; i < data.Length; ++i)
                {
                    txt = txt + " " + data[i].ToString();
                }
                txt += "]";
            }

            f.WriteLine(txt);
            f.Close();

        }

    }
}