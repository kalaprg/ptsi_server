using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ptsi_web
{
    public partial class patienthistory : System.Web.UI.Page
    {

        protected void CreateImage(string filename, byte[] data, System.Drawing.Color c)
        {

            int width = data.Length;
            int height = 256;
            System.Drawing.Pen pen = new System.Drawing.Pen(c);
            System.Drawing.Bitmap image = new System.Drawing.Bitmap(width, height);
            System.Drawing.Graphics g = System.Drawing.Graphics.FromImage(image);

            g.Clear(System.Drawing.Color.White);

            

            for (int i = 0; i < width - 1; ++i)
            {
                g.DrawLine(pen, i, height - 1 - data[i], i + 1, height - 1 - data[i + 1]);
            }

            image.Save(filename);

        }

        protected void Page_Load(object sender, EventArgs e)
        {
            int ID = Convert.ToInt32(Request.QueryString["id"]);

            PatientData p = PatientData.GetByIndex(ID);

            DataReader drECG = new DataReaderDatabase(p.GetPESEL(), DataReader.DataType.ECG);
            DataReader drPulse = new DataReaderDatabase(p.GetPESEL(), DataReader.DataType.Pulse);
            DataReader drOxygen = new DataReaderDatabase(p.GetPESEL(), DataReader.DataType.Oxygen);

            drECG.GetData();
            drPulse.GetData();
            drOxygen.GetData();

            //byte[] dataECG = { 1, 2, 3, 40, 50, 60, 70, 80, 90 };
            //byte[] dataPulse = { 1, 2, 3, 40, 50, 60, 70, 80, 90 };
            //byte[] dataOxygen = { 1, 2, 3, 40, 50, 60, 70, 80, 90 };

            byte[] dataECG = drECG.ReadAll();
            byte[] dataPulse = drPulse.ReadAll();
            byte[] dataOxygen = drOxygen.ReadAll();

            string clientFileECG = "ecg.png";
            string clientFilePulse = "pulse.png";
            string clientFileOxygen = "oxygen.png";

            string serverFileECG = Server.MapPath(clientFileECG);
            string serverFilePulse = Server.MapPath(clientFilePulse);
            string serverFileOxygen = Server.MapPath(clientFileOxygen);

            CreateImage(serverFileECG, dataECG, System.Drawing.Color.Red);
            CreateImage(serverFilePulse, dataPulse, System.Drawing.Color.Green);
            CreateImage(serverFileOxygen, dataOxygen, System.Drawing.Color.Blue);


            imageECG.ImageUrl = clientFileECG;
            imagePulse.ImageUrl = clientFilePulse;
            imageOxygen.ImageUrl = clientFileOxygen;


        }
    }
}