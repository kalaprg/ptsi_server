using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using MySql.Data.MySqlClient;

namespace ptsi_web
{

    public class DatabaseConnection
    {
        private static int count = 0;

        private static MySqlConnection connection = new MySqlConnection();
        private static bool isOpen = false;

        private static Log defaultLog = new Log("database.txt");

        private static string connectionString;

        private static void InitConnection()
        {
            ConfigParser p = Global.parser;

            string ip = p.GetValue("database", "IP");
            string name = p.GetValue("database", "name");
            string user = p.GetValue("database", "user");
            string password = p.GetValue("database", "password");
            connectionString = "SERVER=" + ip + ";" + "DATABASE=" +
                                         name + ";" + "UID=" + user + ";" +
                                        "PASSWORD=" + password + ";";
            
        }

        public DatabaseConnection()
        {
            ++count;
            if (!isOpen)
            {
                InitConnection();
                
                isOpen = true;
                connection.ConnectionString = connectionString;
                connection.Open();
                defaultLog.Write("Connected to: " + connectionString);
            }
        }

        ~DatabaseConnection()
        {

            --count;
            if (count <= 0)
            {
                connection.Close();
                defaultLog.Write("Connection closed");
                isOpen = false;
            }
        }

        public byte[] ReadData(UInt64 PESEL, DataReader.DataType dataType, uint offset = 0)
        {
            string typeString = ((byte)dataType).ToString();

            string query = "SELECT d.data_blob FROM data d JOIN session s ON s.session_id = d.session_id WHERE d.start_date = (SELECT start_date FROM data WHERE data_type = " + typeString + " ORDER BY start_date DESC LIMIT 1 OFFSET "+offset+")";

            defaultLog.Write("Executing query: " + query);

            MySqlCommand cmd = new MySqlCommand(query, connection);

            MySqlDataReader data = cmd.ExecuteReader();

            byte[] outData = new byte[0];

            while (data.Read())
            {
                outData = (byte[])data["data_blob"];
            }

            data.Close();


            defaultLog.Write("...finished: ");
            return outData;

        }

        public byte[] ReadHistory(UInt64 PESEL, DataReader.DataType type)
        {
            byte[] outHistory = new byte[0];

            string typeString = ((byte)type).ToString();
            string query = "SELECT d.data_blob FROM data d JOIN session s ON s.session_id = d.session_id WHERE data_type = " + typeString +" ORDER BY start_date ASC";
            defaultLog.Write("Executing query: " + query);
            MySqlCommand cmd = new MySqlCommand(query, connection);
            MySqlDataReader data = cmd.ExecuteReader();

            while (data.Read())
            {
                outHistory.Concat( (byte[])data["data_blob"] );
            }

            data.Close();

            return outHistory;
        }

        public int ValidatePassword(string login, string password)
        {
            defaultLog.Write("Validating user: " + login);

            string query = "SELECT user_id, password FROM users WHERE login = '"+login+"'";

            MySqlCommand cmd = new MySqlCommand(query, connection);
            MySqlDataReader data = cmd.ExecuteReader();

            string goodPassword = "";
            int userID = 0;

            while (data.Read())
            {
                goodPassword = (string)data["password"];
                userID = (int)data["user_id"];
            }

            data.Close();

            bool passcmp = String.Compare(password, goodPassword) == 0;
            bool notnull = (login.Length != 0  && password.Length != 0);

            if( passcmp && notnull && userID > 0)
                return userID;
            else
                return 0;
        }

        public void LoadPatientData()
        {
            defaultLog.Write("Loading patient data");
            string query = "SELECT PESEL, first_name, last_name FROM patient_data";

            MySqlCommand cmd = new MySqlCommand(query, connection);
            MySqlDataReader data = cmd.ExecuteReader();

            while (data.Read())
            {
                UInt64 PESEL = Convert.ToUInt64((string)data["PESEL"]);
                string first_name = (string)data["first_name"];
                string last_name = (string)data["last_name"];

                PatientData p = new PatientData(first_name, last_name, PESEL, Global.dataSource);
            }

            data.Close();
        }

    }
}