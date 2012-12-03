using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Net.Sockets;

using System.Drawing;

namespace ptsi_web
{
    

    

    public partial class patientview : System.Web.UI.Page
    {
        private static Log log = new Log("patientview.txt");

        ~patientview()
        {
            log.Write("destructor " + n.ToString());
            ++n;
        }

        public static int w;
        public static int h;

        public static int n = 0;

        public patientview()
        {
            //++n;
            log.Write("constructor");
            w = Convert.ToInt32(Global.parser.GetValue("frame", "width"));
            h = Convert.ToInt32(Global.parser.GetValue("frame", "height"));
        }

        protected void Page_Load(object sender, EventArgs e)
        {
            if (Session["UserID"] == null)
            {
                Response.Redirect("~/loginpage.aspx");
            }
            else
            {
                int patientCount = PatientData.GetCount();

                List<int> patientList = new List<int>(patientCount);

                for (int i = 0; i < patientCount; ++i)
                {
                    if (PatientData.GetByIndex(i).observation)
                    {
                        patientList.Add(i);
                    }
                }

                iframeRepeat.DataSource = patientList;
                iframeRepeat.DataBind();
            }
        }
    }
}