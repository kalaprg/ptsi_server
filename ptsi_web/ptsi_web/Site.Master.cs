using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ptsi_web
{
    public partial class SiteMaster : System.Web.UI.MasterPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            bool logged = (Session["UserID"] != null);

            if (logged)
            {
                PanelUserLogged.Visible = true;
                PanelUserNotLogged.Visible = false;
                UserPanel.Text = (string)Session["UserName"];
            }
            else
            {
                PanelUserLogged.Visible = false;
                PanelUserNotLogged.Visible = true;

            }

            DateLabel.Text = DateTime.Today.ToShortDateString();
        }
    }
}
