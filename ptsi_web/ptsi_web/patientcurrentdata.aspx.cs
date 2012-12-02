using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using System.Drawing.Imaging;

namespace ptsi_web
{
    public partial class patientcurrentdata : System.Web.UI.Page
    {
        public string result = "";
        protected void Page_Load(object sender, EventArgs e)
        {
            int ID = Convert.ToInt32( Request.QueryString["id"]);
            DataReader.DataType dataType = (DataReader.DataType)Convert.ToInt32(Request.QueryString["datatype"]);

            PatientData p = PatientData.GetByIndex(ID);

         
            p.UpdateSample(dataType);


            

            //image.Save("test.png");

            Response.Write(p.UpdateLast(dataType));
            //Response.Write(p.Update(dataType));

        }
    }
}