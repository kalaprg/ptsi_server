#ifndef BIOSIGNALDATA_H
#define BIOSIGNALDATA_H
#include <string>
#include <fstream>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cppconn/prepared_statement.h>
#include "common.h"

class Session;

class BiosignalData
{
public:
    typedef boost::shared_ptr<BiosignalData> pointer;
    enum DataType{OXYGEN = 0, PULSE, ECG};

    BiosignalData(const Session &session);
    void storePacket(const DataPacket &packet);
    size_t getSize(unsigned char type, unsigned char readerId);
    void init(unsigned char type, unsigned char readerId, size_t size);
    void readBytes(unsigned char type, unsigned char readerId, std::vector<unsigned char> &buffer);
    void checkAssign(BiosignalData::pointer rhs);

private:
    struct Reader
    {
        Reader(unsigned char readerId) : pos_(0), readerId_(readerId), prev_(false) {}
        std::fstream::pos_type pos_;
        unsigned char readerId_;
        bool prev_;
    };

    class DataBlock : public boost::enable_shared_from_this<DataBlock>
    {
    public:
        typedef boost::shared_ptr<DataBlock> pointer;
        DataBlock(const BiosignalData& biosignalData, double samplingFreq, size_t frameSize, int type,
                  boost::posix_time::ptime startTime = boost::posix_time::second_clock::local_time());

        ~DataBlock();

        void storeByte(unsigned char byte, boost::uint32_t frameNum);
        void storeBytes(const std::vector<unsigned char> &bytes, boost::uint32_t frameNum);
        size_t getSize(unsigned char readerId);
        void init(unsigned char readerId, size_t size);
        void readBytes(unsigned char readerId, std::vector<unsigned char> &buffer);
        void release();

    private:
        void createDataBlock();
        void createNewDataBlock();
        void closeDataBlock();

        boost::posix_time::ptime frameNumToTime(boost::uint32_t frameNum) const;
        int frameNumToPos(boost::uint32_t frameNum) const;

        const BiosignalData& biosignalData_;
        double samplingFreq_;
        size_t frameSize_;
        int firstFrame_;
        boost::posix_time::ptime startTime_;
        boost::filesystem::path fileName_;
        std::fstream file_;
        int type_;
        pointer prev_;
        int prevCount_;
        typedef std::map<unsigned char, Reader> ReadersMap;
        ReadersMap readers_;
        bool released_;

        static const size_t blobSize_ = 16 * 1024;
        friend class BiosignalData;
    };
    void prepareStatement();

    boost::shared_ptr<sql::PreparedStatement> prepStmt_;
    DataBlock::pointer dataBlocks_[3];
    const Session &session_;

};

#endif // BIOSIGNALDATA_H
