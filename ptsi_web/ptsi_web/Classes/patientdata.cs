using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace ptsi_web
{
    public class PatientData
    {
        private static int count = 0;
        private static List<PatientData> list = new List<PatientData>();

        public static PatientData GetByIndex(int index) { return list[index]; }
        public static int GetCount() {return count;}


        private int ID;
        private DataViewer dataViewerPulse = new DataViewer();
        private DataViewer dataViewerOxygen = new DataViewer();
        private DataViewer dataViewerECG = new DataViewer();
        private DataReader dataReaderPulse;
        private DataReader dataReaderOxygen;
        private DataReader dataReaderECG;
        private UInt64 PESEL;
        private string firstName;
        private string lastName;


        public PatientData(string firstName_, string lastName_,
                            UInt64 PESEL_, DataReader.DataSource source)
        {
            firstName = firstName_;
            lastName = lastName_;
            PESEL = PESEL_;

            string pECG = "E:/Studia/Semestr7/PTSI/L/git/ptsi_server/randomdata/ecg.txt";
            string pPulse = "E:/Studia/Semestr7/PTSI/L/git/ptsi_server/randomdata/pulse.txt";
            string pOxygen = "E:/Studia/Semestr7/PTSI/L/git/ptsi_server/randomdata/oxygen.txt";

            switch (source)
            {
                case DataReader.DataSource.File:
                    dataReaderPulse = new DataReaderFile(PESEL_, DataReader.DataType.Pulse, pPulse);
                    dataReaderOxygen = new DataReaderFile(PESEL_, DataReader.DataType.Oxygen, pOxygen);
                    dataReaderECG = new DataReaderFile(PESEL_, DataReader.DataType.ECG, pECG);
                    break;
                case DataReader.DataSource.Server:
                    dataReaderPulse = new DataReaderServer(PESEL_, DataReader.DataType.Pulse);
                    dataReaderOxygen = new DataReaderServer(PESEL_, DataReader.DataType.Oxygen);
                    dataReaderECG = new DataReaderServer(PESEL_, DataReader.DataType.ECG);
                    break;
                case DataReader.DataSource.Database:
                    dataReaderPulse = new DataReaderDatabase(PESEL_, DataReader.DataType.Pulse);
                    dataReaderOxygen = new DataReaderDatabase(PESEL_, DataReader.DataType.Oxygen);
                    dataReaderECG = new DataReaderDatabase(PESEL_, DataReader.DataType.ECG);
                    break;
            }

            list.Add(this);

            ID = count;
            ++count;
        }

        public string GetFirstName() {return firstName;}
        public string GetLastName() { return lastName; }
        public UInt64 GetPESEL() { return PESEL; }
        public byte[] GetDataECG() { return dataViewerECG.GetBytes();}
        public byte[] GetDataPulse() { return dataViewerPulse.GetBytes(); }
        public byte[] GetDataOxygen() { return dataViewerOxygen.GetBytes(); }


        //read fragment of data into the data reader
        public void ReadData()
        {
            dataReaderECG.GetData();
            dataReaderOxygen.GetData();
            dataReaderPulse.GetData();
        }

        //return one byte from data reader
        private byte ReadSampleECG(){ return dataReaderECG.ReadByte(); }
        private byte ReadSamplePulse() { return dataReaderPulse.ReadByte(); }
        private byte ReadSampleOxygen() { return dataReaderOxygen.ReadByte(); }

        //add one byte to data viewer
        public void UpdateSampleECG() { dataViewerECG.Add(ReadSampleECG()); }
        public void UpdateSamplePulse() { dataViewerPulse.Add(ReadSamplePulse()); }
        public void UpdateSampleOxygen() { dataViewerOxygen.Add(ReadSampleOxygen()); }

        //return current data from data viewer (to generate server response)
        public string UpdateECG() { return dataViewerECG.ToString(); }
        public string UpdatePulse() { return dataViewerPulse.ToString(); }
        public string UpdateOxygen() { return dataViewerOxygen.ToString(); }

        //return only last byte of the current data
        public string UpdateLastECG() { return dataViewerECG.GetLast(); }
        public string UpdateLastPulse() { return dataViewerPulse.GetLast(); }
        public string UpdateLastOxygen() { return dataViewerOxygen.GetLast(); }


        public void UpdateSample(DataReader.DataType dataType)
        {
            switch (dataType)
            {
                case DataReader.DataType.ECG:
                    UpdateSampleECG();
                    break;
                case DataReader.DataType.Oxygen:
                    UpdateSampleOxygen();
                    break;
                case DataReader.DataType.Pulse:
                    UpdateSamplePulse();
                    break;
            }
        }

        public string Update(DataReader.DataType dataType)
        {
            switch(dataType)
            {
                case DataReader.DataType.ECG:
                    return UpdateECG();
                case DataReader.DataType.Oxygen:
                    return UpdateOxygen();
                case DataReader.DataType.Pulse:
                    return UpdatePulse();
                default:
                    return null;
            }
        }

        public string UpdateLast(DataReader.DataType dataType)
        {
            switch(dataType)
            {
                case DataReader.DataType.ECG:
                    return UpdateLastECG();
                case DataReader.DataType.Oxygen:
                    return UpdateLastOxygen();
                case DataReader.DataType.Pulse:
                    return UpdateLastPulse();
                default:
                    return null;
            }
        }



    }
}