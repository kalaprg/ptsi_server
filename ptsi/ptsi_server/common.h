#ifndef COMMON_H
#define COMMON_H
#include <boost/enable_shared_from_this.hpp>
#include <boost/cstdint.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
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

class PTSIOptions
{
public:
    PTSIOptions();
    bool parseConfigFile(const boost::filesystem::path &configFile);
    void printUsage(std::ostream &stream);

    size_t getBlobSize() const;
    bool isUnsecuredConnectionsEnabled() const;
    unsigned short getPort() const;
    bool isSecuredConnectionsEnabled() const;
    unsigned short getSecurePort() const;
    unsigned short getInternalPort() const;
    const std::string &getMySQLServer() const;
    const std::string &getMySQLUser() const;
    const std::string &getMySQLPassword() const;
    const std::string &getMySQLDatabase() const;
private:
    boost::program_options::options_description options_;
    boost::program_options::options_description generalOptions_;
    boost::program_options::options_description mysqlOptions_;
    boost::program_options::variables_map vm_;
    std::string mysqlServer_;
    std::string mysqlUser_;
    std::string mysqlPassword_;
    std::string mysqlDatabase_;
    size_t blobSizeKiB_;
    unsigned short port_;
    unsigned short internalPort_;
    unsigned short securePort_;
};

extern PTSIOptions globalOptions;

#endif // COMMON_H
