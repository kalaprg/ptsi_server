#include <sstream>
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <boost/scoped_ptr.hpp>
#include "ptsiserver.h"
#include "session.h"

Session::Session(const PTSIServer &server)
    : server_(server), session_id_(-1), deviceName_(""), pesel_(0),
      samplingFrequency_(0), frameSize_(0)
{
}

bool Session::associateWithPatient(const boost::uint64_t &pesel,
                                   const std::string &deviceName)
{
    std::string query;
    try
    {
        std::stringstream s;
        s << pesel;
        std::string peselString(s.str());
        boost::shared_ptr<sql::Connection> conn(server_.databaseConnection());
        if(!conn)
        {
            server_.errorStream_ << "Can't associate - no database connection!" << std::endl;
            return false;
        }

        boost::scoped_ptr<sql::Statement> stmt(conn->createStatement());

        closeSession();//close old session;

        query = "SELECT session_id FROM session "
                "WHERE PESEL='" + peselString + "' AND end_date IS NULL";
        {
            boost::scoped_ptr<sql::ResultSet> res(stmt->executeQuery(query));

            if(res && res->next())
            {
                server_.debugStream_ << "Session #" << res->getInt("session_id")
                                     << "is already associated with " << peselString
                                     << "pesel" << std::endl;
                return false;
            }
        }
        query = "INSERT INTO session(device_name, PESEL, start_date) "
                "VALUES('" + deviceName + "', '" + peselString + "', NOW())";

        stmt->executeUpdate(query);

        query = "SELECT LAST_INSERT_ID() AS id FROM session;";
        {
            boost::scoped_ptr<sql::ResultSet> res(stmt->executeQuery(query));
            if(res && res->next())
            {
                session_id_ = res->getInt("id");
                deviceName_ = deviceName;
                pesel_ = pesel;
                return true;
            }
            else
            {
                server_.errorStream_ << "Can't associate - no LAST_INSERT_ID()" << std::endl;
                return false;
            }
        }

    }
    catch(sql::SQLException &e)
    {
        server_.errorStream_ << "Error executing query: \"" << query << "\":"
                             << e.what() << e.getErrorCode() << std::endl;
    }
    return false;
}

bool Session::setupTransmission(const TransmissionSetup &setup)
{
    //TODO
    //if already setup, create new session(in db) and flush blobs
    if(!isAssociated() || !setup.ecgFrameSize_ || !setup.samplingFrequency_)
        return false;

    if(!isTransmissionSetup())
    {
        std::string query;
        try
        {
            boost::shared_ptr<sql::Connection> conn(server_.databaseConnection());
            if(!conn)
            {
                server_.errorStream_ << "Can't setup transmission - no database connection!" << std::endl;
                return false;
            }

            boost::scoped_ptr<sql::Statement> stmt(conn->createStatement());

            {
                std::stringstream s;
                s << "UPDATE session SET "
                     "sampling_frequency='" << setup.samplingFrequency_ << "', "
                     "frame_size='" << setup.ecgFrameSize_ << "' "
                     "WHERE session_id='" << session_id_ << "'";

                query = s.str();
            }

            if(!stmt->executeUpdate(query))
            {
                server_.errorStream_ << "Can't setup transmission - no session #" << session_id_ << std::endl;
                return false;
            }

            samplingFrequency_ = setup.samplingFrequency_;
            frameSize_ = setup.ecgFrameSize_;

            data_ = BiosignalData::pointer(new BiosignalData(*this));
            return true;

        }
        catch(sql::SQLException &e)
        {
            server_.errorStream_ << "Error executing query: \"" << query << "\":"
                                 << e.what() << std::endl;
        }
        return false;
    }
    else
    {
        if(setup.ecgFrameSize_ == frameSize_ && setup.samplingFrequency_ == samplingFrequency_)
        {
            BiosignalData::pointer newData(new BiosignalData(*this));
            data_->checkAssign(newData);
            data_ = newData;
            return true;
        }

        return false;
    }
}

bool Session::isAssociated() const
{
    return session_id_ != -1;
}

bool Session::isTransmissionSetup() const
{
    return samplingFrequency_ && frameSize_;
}

void Session::closeSession()
{
    if(isAssociated())
    {
        std::string query;
        try
        {
            boost::shared_ptr<sql::Connection> conn(server_.databaseConnection());
            if(!conn)
            {
                server_.errorStream_ << "Can't close session - no database connection!" << std::endl;
                return;
            }

            boost::scoped_ptr<sql::Statement> stmt(conn->createStatement());

            {
                std::stringstream s;
                s << "UPDATE session SET end_date=NOW() "
                     "WHERE session_id='" << session_id_ << "'";

                query = s.str();
            }

            if(!stmt->executeUpdate(query))
            {
                server_.errorStream_ << "Can't close session - no session #" << session_id_ << std::endl;
                return;
            }

            session_id_ = -1;
            pesel_ = 0;
            deviceName_ = "";
            samplingFrequency_ = 0;
            frameSize_ = 0;

        }
        catch(sql::SQLException &e)
        {
            server_.errorStream_ << "Error executing query: \"" << query << "\":"
                                 << e.what() << std::endl;
        }
    }
}

const std::string &Session::getDeviceName() const
{
    return deviceName_;
}

boost::uint64_t Session::getPesel() const
{
    return pesel_;
}

boost::uint16_t Session::getSamplingFrequency() const
{
    return samplingFrequency_;
}

boost::uint16_t Session::getFrameSize() const
{
    return frameSize_;
}

int Session::getSessionID() const
{
    return session_id_;
}

const PTSIServer &Session::getServer() const
{
    return server_;
}

BiosignalData::pointer Session::getData()
{
    return data_;
}

void Session::getUnexpiredSessions(const PTSIServer &server, std::list<pointer> &sessions)
{
    sessions.clear();
    std::string query;
    try
    {
        boost::shared_ptr<sql::Connection> conn(server.databaseConnection());
        if(!conn)
        {
            server.errorStream_ << "Can't get unexpired sessions - no database connection!" << std::endl;
            return;
        }

        boost::scoped_ptr<sql::Statement> stmt(conn->createStatement());

        query = "SELECT session_id, PESEL, device_name, sampling_frequency, frame_size "
                "FROM session "
                "WHERE end_date IS NULL";

        boost::scoped_ptr<sql::ResultSet> res(stmt->executeQuery(query));
        while(res->next())
        {
            pointer session = create(server);
            session->session_id_        = res->getInt("session_id");
            session->pesel_             = res->getInt64("PESEL");
            session->deviceName_        = res->getString("device_name");
            session->samplingFrequency_ = res->getInt("sampling_frequency");
            session->frameSize_         = res->getInt("frame_size");            
            session->data_ = BiosignalData::pointer(new BiosignalData(*session));
            sessions.push_back(session);
        }

    }
    catch(sql::SQLException &e)
    {
        server.errorStream_ << "Error executing query: \"" << query << "\":"
                             << e.what() << std::endl;
    }
}

Session::pointer Session::create(const PTSIServer &server)
{
    return pointer(new Session(server));
}
