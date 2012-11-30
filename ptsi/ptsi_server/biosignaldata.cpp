#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time/int_adapter.hpp>
#include <locale>
#include <sstream>
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "ptsiserver.h"
#include "session.h"
#include "biosignaldata.h"

BiosignalData::BiosignalData(const Session &session)
    : session_(session)
{
    prepareStatement();
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    double pulseFs, oxyFs;
    pulseFs = oxyFs = session_.getSamplingFrequency() / session_.getFrameSize();
    dataBlocks_[OXYGEN] = DataBlock::pointer(new DataBlock(*this, oxyFs, 1, OXYGEN, now));
    dataBlocks_[PULSE] = DataBlock::pointer(new DataBlock(*this, pulseFs, 1, PULSE, now));
    dataBlocks_[ECG] = DataBlock::pointer(
                new DataBlock(*this, session_.getSamplingFrequency(),
                              session_.getFrameSize(), ECG, now));
}

void BiosignalData::storePacket(const DataPacket &packet)
{
    dataBlocks_[OXYGEN]->storeByte(packet.saturation_, packet.frameNumber_);
    dataBlocks_[PULSE]->storeByte(packet.pulse_, packet.frameNumber_);
    dataBlocks_[ECG]->storeBytes(packet.ecg_, packet.frameNumber_);
}

size_t BiosignalData::getSize(unsigned char type, unsigned char readerId)
{
    assert(type < 3);
    return dataBlocks_[type]->getSize(readerId);
}

void BiosignalData::init(unsigned char type, unsigned char readerId, size_t size)
{
    assert(type < 3);
    dataBlocks_[type]->init(readerId, size);
}

void BiosignalData::readBytes(unsigned char type, unsigned char readerId, std::vector<unsigned char> &buffer)
{
    assert(type < 3);
    dataBlocks_[type]->readBytes(readerId, buffer);
}

void BiosignalData::checkAssign(BiosignalData::pointer rhs)
{
    if(dataBlocks_[OXYGEN]->fileName_ == rhs->dataBlocks_[OXYGEN]->fileName_)
        dataBlocks_[OXYGEN]->release();

    if(dataBlocks_[PULSE]->fileName_ == rhs->dataBlocks_[PULSE]->fileName_)
        dataBlocks_[PULSE]->release();

    if(dataBlocks_[ECG]->fileName_ == rhs->dataBlocks_[ECG]->fileName_)
        dataBlocks_[ECG]->release();
}

void BiosignalData::prepareStatement()
{
    std::string query;
    try
    {
        boost::shared_ptr<sql::Connection> conn(session_.getServer().databaseConnection());
        if(!conn)
        {
            session_.getServer().errorStream_ << "Can't close data block - no database connection!" << std::endl;
            return;
        }

        query = "UPDATE data SET data_blob=?, file_path=NULL WHERE session_id=? AND start_date=? AND data_type=?";
        prepStmt_ = boost::shared_ptr<sql::PreparedStatement>(conn->prepareStatement(query));

    }
    catch(sql::SQLException &e)
    {
        session_.getServer().errorStream_ << "Error executing query: \"" << query << "\":"
                                          << e.what() << std::endl;
    }
}

BiosignalData::DataBlock::DataBlock(const BiosignalData &biosignalData,
                                    double samplingFreq, size_t frameSize, int type,
                                    boost::posix_time::ptime startTime)
    : biosignalData_(biosignalData), samplingFreq_(samplingFreq),
      frameSize_(frameSize), firstFrame_(-1), startTime_(startTime),
      fileName_(boost::filesystem::temp_directory_path()), type_(type),
      prevCount_(0), released_(false)
{
    fileName_ /= boost::filesystem::unique_path("ptsi-%%%%-%%%%-%%%%-%%%%");
    createDataBlock();
}

BiosignalData::DataBlock::~DataBlock()
{
    if(!released_)
    {
        closeDataBlock();
        file_.close();
        boost::filesystem::remove(fileName_);
    }
}

void BiosignalData::DataBlock::storeByte(unsigned char byte, boost::uint32_t frameNum)
{
    if(firstFrame_ == -1)
        firstFrame_ = frameNum;

    int pos = frameNumToPos(frameNum);
    if(pos < 0)
        return; // too old frames
    if(pos >= (int)blobSize_)
        return; // TODO fork

    if(pos < file_.tellp())
    {
        int curr_pos = file_.tellp();
        file_.seekp(pos, std::ios_base::beg);
        file_.put(byte);
        file_.seekp(curr_pos, std::ios_base::beg);
    }
    else if(pos > file_.tellp())
    {
        int curr_pos = file_.tellp();
        for(int i = curr_pos; i < pos; ++i)
            file_.put(255);//fill with no signal values

        file_.put(byte);
        file_.seekp(curr_pos, std::ios_base::beg);
    }
    else
    {
        file_.put(byte);
    }
}

void BiosignalData::DataBlock::storeBytes(const std::vector<unsigned char> &bytes, boost::uint32_t frameNum)
{
    if(firstFrame_ == -1)
        firstFrame_ = frameNum;

    int pos = frameNumToPos(frameNum);
    if(pos < 0)
        return; // too old frames
    if(pos >= (int)blobSize_)
        return; // TODO fork

    if(pos < file_.tellp())
    {
        int curr_pos = file_.tellp();
        file_.seekp(pos, std::ios_base::beg);

        size_t bytesLeft = blobSize_ - curr_pos;
        file_.write((const char*)&bytes.front(), bytesLeft);

        if(bytesLeft < bytes.size())
        {
            //TODO fork
            return;
        }
        else
        {
            file_.seekp(curr_pos, std::ios_base::beg);
        }
    }
    else if(pos > file_.tellp())
    {
        int curr_pos = file_.tellp();
        for(int i = curr_pos; i < pos; ++i)
            file_.put(255);//fill with no signal values

        size_t bytesLeft = blobSize_ - curr_pos;
        file_.write((const char*)&bytes.front(), bytesLeft);

        if(bytesLeft < bytes.size())
        {
            //TODO fork
            return;
        }
        else
        {
            file_.seekp(curr_pos, std::ios_base::beg);
        }
    }
    else
    {
        int curr_pos = file_.tellp();
        size_t bytesLeft = blobSize_ - curr_pos;
        file_.write((const char*)&bytes.front(), std::min(bytesLeft, bytes.size()));
        if(bytesLeft < bytes.size())
        {
            //TODO fork
            return;
        }
    }
}

size_t BiosignalData::DataBlock::getSize(unsigned char readerId)
{
    ReadersMap::iterator it = readers_.find(readerId);
    if(it == readers_.end())
    {
        it = readers_.insert(ReadersMap::value_type(readerId, Reader(readerId))).first;
    }

    size_t size = 0;
    if(it->second.prev_)
    {
        size = prev_->getSize(readerId);
        if(size == 0)
        {
            it->second.prev_ = false;
            if(!(--prevCount_))
            {
                prev_.reset();
            }
        }
    }
    size += file_.tellp() - it->second.pos_;
    return size;
}

void BiosignalData::DataBlock::init(unsigned char readerId, size_t size)
{
    ReadersMap::iterator it = readers_.find(readerId);
    if(it == readers_.end())
    {
        it = readers_.insert(ReadersMap::value_type(readerId, Reader(readerId))).first;
    }

    if(it->second.prev_)
    {
        it->second.prev_ = false;
        if(!(--prevCount_))
        {
            prev_.reset();
        }
    }

    int newPos = (size_t)file_.tellp() - size;
    it->second.pos_ = (newPos < 0) ? 0 : newPos;
}

void BiosignalData::DataBlock::readBytes(unsigned char readerId, std::vector<unsigned char> &buffer)
{
    ReadersMap::iterator it = readers_.find(readerId);
    if(it == readers_.end())
    {
        it = readers_.insert(ReadersMap::value_type(readerId, Reader(readerId))).first;
    }

    size_t size = 0;
    int offset = 0;
    if(it->second.prev_)
    {
        size = prev_->getSize(readerId);
        if(size == 0)
        {
            it->second.prev_ = false;
            if(!(--prevCount_))
            {
                prev_.reset();
            }
        }
        else
        {
            if(buffer.size() > size)
            {
                prev_->readBytes(readerId, buffer);
                offset = size;
            }
            else
            {
                prev_->readBytes(readerId, buffer);
                return;
            }
        }
    }
    size = file_.tellp() - it->second.pos_;
    file_.seekg(it->second.pos_);
    file_.read((char *)&buffer[offset], std::min(size, buffer.size() - offset));
    size_t remaining = buffer.size() - offset - size;
    if(remaining)
        memset(&buffer[offset + size], 255, remaining);

    it->second.pos_ = file_.tellg();
}

void BiosignalData::DataBlock::release()
{
    released_ = true;
}

void BiosignalData::DataBlock::createDataBlock()
{
    if(released_)
        return;

    std::string query;
    try
    {
        boost::shared_ptr<sql::Connection> conn(biosignalData_.session_.getServer().databaseConnection());
        if(!conn)
        {
            biosignalData_.session_.getServer().errorStream_ << "Can't create data block - no database connection!" << std::endl;
            return;
        }

        boost::scoped_ptr<sql::Statement> stmt(conn->createStatement());
        std::stringstream s;
        s << "SELECT file_path, start_date FROM data "
             "WHERE session_id=" << biosignalData_.session_.getSessionID()
          << " AND data_type=" << type_ << " AND file_path IS NOT NULL";

        query = s.str();
        boost::scoped_ptr<sql::ResultSet> res(stmt->executeQuery(query));
        if(res && res->next())
        {
            fileName_ = res->getString("file_path").asStdString();
            file_.exceptions(std::ios_base::eofbit | std::ios_base::badbit | std::ios_base::failbit);
            std::ios_base::openmode mode(std::ios_base::in | std::ios_base::out | std::ios_base::binary | std::ios_base::ate);
            file_.open(fileName_.c_str(), mode);

            startTime_ = boost::posix_time::time_from_string(res->getString("start_date").asStdString());
        }
        else
            createNewDataBlock();

        //TODO maybe throw exception in case of error?

    }
    catch(sql::SQLException &e)
    {
        biosignalData_.session_.getServer().errorStream_ << "Error executing query: \"" << query << "\":"
                                                         << e.what() << std::endl;
    }
    catch(std::ios_base::failure &e)
    {
        biosignalData_.session_.getServer().errorStream_ << "fstream open error:" << e.what() << std::endl;
    }
}

void BiosignalData::DataBlock::createNewDataBlock()
{
    if(released_)
        return;

    std::string query;
    try
    {
        boost::shared_ptr<sql::Connection> conn(biosignalData_.session_.getServer().databaseConnection());
        if(!conn)
        {
            biosignalData_.session_.getServer().errorStream_ << "Can't create data block - no database connection!" << std::endl;
            return;
        }

        boost::scoped_ptr<sql::Statement> stmt(conn->createStatement());
        std::stringstream s;
        s << "INSERT INTO data(session_id, start_date, data_type, file_path) "
             "VALUES(" << biosignalData_.session_.getSessionID() << ", \'";

        s.imbue(std::locale(std::cout.getloc(), new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S")));
        s << startTime_ << "', " << (int) type_ << ", " << fileName_ << ')';
        query = s.str();
        stmt->executeUpdate(query);

        file_.exceptions(std::ios_base::eofbit | std::ios_base::badbit | std::ios_base::failbit);
        std::ios_base::openmode mode(std::ios_base::in | std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        file_.open(fileName_.c_str(), mode);

        //TODO maybe throw exception in case of error?

    }
    catch(sql::SQLException &e)
    {
        biosignalData_.session_.getServer().errorStream_ << "Error executing query: \"" << query << "\":"
                                                         << e.what() << std::endl;
    }
    catch(std::ios_base::failure &e)
    {
        biosignalData_.session_.getServer().errorStream_ << "fstream open error:" << e.what() << std::endl;
    }
}

void BiosignalData::DataBlock::closeDataBlock()
{
    if(released_)
        return;

    try
    {
        boost::shared_ptr<sql::Connection> conn(biosignalData_.session_.getServer().databaseConnection());
        if(!conn)
        {
            biosignalData_.session_.getServer().errorStream_ << "Can't close data block - no database connection!" << std::endl;
            return;
        }

        std::stringstream s;
        s.imbue(std::locale(std::cout.getloc(), new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S")));
        s << startTime_;

        file_.seekg(0, std::ios_base::beg);
        file_.exceptions(std::ios_base::goodbit);//disable exceptions
        biosignalData_.prepStmt_->setBlob(1, &file_);
        biosignalData_.prepStmt_->setInt(2, biosignalData_.session_.getSessionID());
        biosignalData_.prepStmt_->setDateTime(3, sql::SQLString(s.str()));
        biosignalData_.prepStmt_->setInt(4, type_);
        biosignalData_.prepStmt_->executeUpdate();
        file_.exceptions(std::ios_base::eofbit | std::ios_base::badbit | std::ios_base::failbit);//reenable exceptions

        //TODO maybe throw exception in case of error?

    }
    catch(std::ios_base::failure &e)
    {
        biosignalData_.session_.getServer().errorStream_ << "fstream error:" << e.what() << std::endl;
    }
    catch(boost::filesystem::filesystem_error &e)
    {
        biosignalData_.session_.getServer().errorStream_ << "Filesystem error:" << e.what() << std::endl;
    }
    catch(sql::SQLException &e)
    {
        biosignalData_.session_.getServer().errorStream_ << "Error executing prepared statement:"
                                          << e.what() << std::endl;
    }
}

boost::posix_time::ptime BiosignalData::DataBlock::frameNumToTime(boost::uint32_t frameNum) const
{
    using namespace boost::posix_time;
    using namespace boost::date_time;
    assert(firstFrame_ != -1);//this should be set before calling this method
    int offset = frameNum - firstFrame_;
    if(offset < 0)
        return ptime(neg_infin);//frame before this data block

    size_t samples = offset * frameSize_;
    if(samples >= blobSize_)
        return ptime(pos_infin);//frame after this data block

    double usecs = samples * samplingFreq_ * 1e-9;
    return ptime(startTime_ + microseconds(usecs));
}

int BiosignalData::DataBlock::frameNumToPos(boost::uint32_t frameNum) const
{
    int offset = frameNum - firstFrame_;
    size_t samples = offset * frameSize_;
    return samples;
}
