using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ptsi_web
{
    public partial class patientframe : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            int ID = Convert.ToInt32(Request.QueryString["id"]);

            if (ID < PatientData.GetCount() && ID >= 0)
            {

                //set the hidden label for javascript to read
                patientID.Value = ID.ToString();

                PatientData p = PatientData.GetByIndex(ID);

                patientFirstName.Text = p.GetFirstName();
                patientLastName.Text = p.GetLastName();
                patientPESEL.Text = "[" + p.GetPESEL().ToString() + "]";

                p.ReadData();
            }
            else
            {
                Response.Write("Niepoprawne dane pacjenta!");
            }

        }
    }
}