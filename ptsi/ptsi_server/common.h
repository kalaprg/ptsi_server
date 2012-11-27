#ifndef COMMON_H
#define COMMON_H
#include <boost/enable_shared_from_this.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <vector>

enum PacketType {SET_PATIENT = 0, SETUP_TRANSMISSION, BIOSIGNALS};

class Connection;

class SetPatientPacket
        : public boost::enable_shared_from_this<SetPatientPacket>
{
public:
    typedef boost::shared_ptr<SetPatientPacket> pointer;
    SetPatientPacket();
    const std::string &login() const;
    const std::string &password() const;
    boost::uint64_t pesel() const;
    friend class Connection;
private:
    std::string login_;
    std::string password_;
    boost::uint64_t pesel_;
};

struct TransmissionSetup
{
    boost::uint16_t samplingFrequency_;
    boost::uint16_t ecgFrameSize_;
};

struct DataPacket
{
    boost::uint32_t frameNumber_;
    unsigned char saturation_;
    unsigned char pulse_;
    std::vector<unsigned char> ecg_;
};

#endif // COMMON_H
