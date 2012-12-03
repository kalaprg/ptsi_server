using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ptsi_web
{
    public partial class patientselect : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {

            if(Session["UserID"] == null)
            {
                Response.Redirect("~/loginpage.aspx");
            }
            else
            {
                if (IsPostBack)
                {
                    //update the observation state of patients
                    int size = PatientSelectRepeater.Items.Count;
                    for (int i = 0; i < size; ++i)
                    {

                        CheckBox c = (CheckBox)PatientSelectRepeater.Items[i].FindControl("PScheckbox");
                        PatientData.GetByIndex(i).observation = c.Checked;
                    }
                }

                List<PatientData> patientList = PatientData.GetList();

                PatientSelectRepeater.DataSource = patientList;
                PatientSelectRepeater.DataBind();
            }
        }
    }
}