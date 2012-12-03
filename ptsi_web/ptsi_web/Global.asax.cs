using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Security;
using System.Web.SessionState;


namespace ptsi_web
{
    public class Global : System.Web.HttpApplication
    {

        public static ConfigParser parser;
        public static DataReader.DataSource dataSource;
        public static string filePathECG;
        public static string filePathPulse;
        public static string filePathOxygen;
        public static string rootPath;

        //private static PatientData p0 = new PatientData("Andrzej", "Kmicic",
        //    44051401458, DataReader.DataSource.File);

        //private static PatientData p1 = new PatientData("Tadeusz", "Soplica",
        //    44051401458, DataReader.DataSource.File);

        //private static PatientData p2 = new PatientData("Cezary", "Baryka",
        //    44051401458, DataReader.DataSource.File);

        //private static PatientData p3 = new PatientData("Kubuś", "Puchatek",
        //    44051401458, DataReader.DataSource.File);

        //private static PatientData p4 = new PatientData("Papa", "Smerf",
        //    44051401458, DataReader.DataSource.File);

        void Application_Start(object sender, EventArgs e)
        {
            parser = new ConfigParser(Server.MapPath("config.cfg"));


            rootPath = Server.MapPath("/");
            filePathECG = Server.MapPath("randomdata/ecg.txt");
            filePathPulse = Server.MapPath("randomdata/pulse.txt");
            filePathOxygen = Server.MapPath("randomdata/oxygen.txt");


            string ds = parser.GetValue("other", "datasource");

            SetDataSource(ds);


            DatabaseConnection database = new DatabaseConnection();
            database.LoadPatientData();
        }

        void Application_End(object sender, EventArgs e)
        {
            //  Code that runs on application shutdown

        }

        void Application_Error(object sender, EventArgs e)
        {
            // Code that runs when an unhandled error occurs

        }

        void Session_Start(object sender, EventArgs e)
        {
            // Code that runs when a new session is started

        }

        void Session_End(object sender, EventArgs e)
        {
            // Code that runs when a session ends. 
            // Note: The Session_End event is raised only when the sessionstate mode
            // is set to InProc in the Web.config file. If session mode is set to StateServer 
            // or SQLServer, the event is not raised.
            
        }

        void SetDataSource(string ds_)
        {
            switch (ds_)
            {
                case "file":
                    dataSource = DataReader.DataSource.File;
                    break;
                case "server":
                    dataSource = DataReader.DataSource.Server;
                    break;
            }

        }

    }
}
