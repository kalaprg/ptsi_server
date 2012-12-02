using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace ptsi_web
{
    public partial class loginpage : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {

            if (Session["UserID"] != null)
            {
                int ID = (int)Session["UserID"];
            }
            else
            {
                if (Page.IsPostBack)
                {
                    string login = UserName.Text;
                    string password = Password.Text;

                    DatabaseConnection database = new DatabaseConnection();

                    int id = database.ValidatePassword(login, password);

                    if (id > 0)
                    {
                        Session["UserID"] = id;
                        Session["UserName"] = login;
                        Response.Redirect("~/Default.aspx");
                    }
                    else
                    {
                        result.Text = "Niepoprawne dane logowania";
                    }
                }
            }
        }

    }
}