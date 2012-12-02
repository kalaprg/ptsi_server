#include <fstream>
#include "common.h"

SetPatientPacket::SetPatientPacket()
    : pesel_(0)
{
}

const std::string &SetPatientPacket::login() const
{
    return login_;
}

const std::string &SetPatientPacket::password() const
{
    return password_;
}

boost::uint64_t SetPatientPacket::pesel() const
{
    return pesel_;
}

PTSIOptions::PTSIOptions()
    : options_("Config file options"), generalOptions_("General options"),
      mysqlOptions_("MySQL options"), sslOptions("TLS options")
{
    using namespace boost::program_options;
    generalOptions_.add_options()
            ("general.blob-size", value<size_t>(&blobSizeKiB_)->default_value(16), "size of data chunk in kilobytes")
            ("general.port", value<unsigned short> (&port_)->required(), "port number for unsecured connections.\n0 disables this functionality")
            ("general.internal-port", value<unsigned short> (&internalPort_)->required(), "port number used for internal connections")
            ("general.secure-port", value<unsigned short>(&securePort_)->required(), "port number for TLS connections.\n0 disables this functionality");

    mysqlOptions_.add_options()
            ("mysql.server", value<std::string>(&mysqlServer_)->default_value("localhost"), "location of MySQL server")
            ("mysql.user", value<std::string> (&mysqlUser_)->required(), "MySQL user name")
            ("mysql.password", value<std::string> (&mysqlPassword_)->required(), "MySQL password")
            ("mysql.db", value<std::string>(&mysqlDatabase_)->default_value("ptsi"), "MySQL database name");

    sslOptions.add_options()
            ("tls.cert", value<boost::filesystem::path>(&certificateFile_), "server certificate file")
            ("tls.key", value<boost::filesystem::path>(&rsaKeyFile_), "server private key file")
            ("tls.keypass", value<std::string>(&keyPassword_)->default_value(""), "private key passphrase");

    options_.add(generalOptions_).add(mysqlOptions_).add(sslOptions);
}

bool PTSIOptions::parseConfigFile(const boost::filesystem::path &configFile)
{
    try
    {
        if(!boost::filesystem::exists(configFile))
        {
            std::cerr << "Config file \"" << configFile << "\" does not exist" << std::endl;
            return false;
        }

        std::ifstream ifs(configFile.c_str());
        boost::program_options::store(
                    boost::program_options::parse_config_file(ifs, options_), vm_);
        boost::program_options::notify(vm_);

        if(!port_ && !securePort_)
        {
            std::cerr << "You cannot disable both unsecured and secured connections";
            return false;
        }

        if(securePort_ && (!vm_.count("tls.cert") || !vm_.count("tls.key")))
        {
            std::cerr << "In order to use TLS you must specify certificate file and public key file!";
            return false;
        }

        return true;
    }
    catch(boost::filesystem::filesystem_error &e)
    {
        std::cerr << "Cannot open config file \"" << configFile << "\": " << e.what() << std::endl;
        return false;
    }
    catch(boost::program_options::error &e)
    {
        std::cerr << "Error parsing config file: " << e.what() << std::endl;
        return false;
    }
}

void PTSIOptions::printUsage(std::ostream &stream)
{
    stream << options_;
}

size_t PTSIOptions::getBlobSize() const
{
    return blobSizeKiB_ * 1024;
}

bool PTSIOptions::isUnsecuredConnectionsEnabled() const
{
    return port_;
}

unsigned short PTSIOptions::getPort() const
{
    return port_;
}

bool PTSIOptions::isSecuredConnectionsEnabled() const
{
    return securePort_;
}

unsigned short PTSIOptions::getSecurePort() const
{
    return securePort_;
}

unsigned short PTSIOptions::getInternalPort() const
{
    return internalPort_;
}

const std::string &PTSIOptions::getMySQLServer() const
{
    return mysqlServer_;
}

const std::string &PTSIOptions::getMySQLUser() const
{
    return mysqlUser_;
}

const std::string &PTSIOptions::getMySQLPassword() const
{
    return mysqlPassword_;
}

const std::string &PTSIOptions::getMySQLDatabase() const
{
    return mysqlDatabase_;
}

const boost::filesystem::path &PTSIOptions::getCertificateFile() const
{
    return certificateFile_;
}

const boost::filesystem::path &PTSIOptions::getPKeyFile() const
{
    return rsaKeyFile_;
}

const std::string &PTSIOptions::getPKeyPassphrase() const
{
    return keyPassword_;
}

PTSIOptions globalOptions;
