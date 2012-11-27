#ifndef SESSION_H
#define SESSION_H
#include <boost/enable_shared_from_this.hpp>
#include <boost/cstdint.hpp>
#include <list>
#include <common.h>

class PTSIServer;

class Session : public boost::enable_shared_from_this<Session>
{
public:
    typedef boost::shared_ptr<Session> pointer;
    static void getUnexpiredSessions(const PTSIServer &server, std::list<pointer> &sessions);
    static pointer create(const PTSIServer &server);

    bool setupTransmission(const TransmissionSetup &setup);

    bool isAssociated() const;
    bool isTransmissionSetup() const;

    const std::string &getDeviceName() const;
    boost::uint64_t getPesel() const;
    boost::uint16_t getSamplingFrequency() const;
    boost::uint16_t getFrameSize() const;

private:
    Session(const PTSIServer &server);

    bool associateWithPatient(const boost::uint64_t &pesel,
                              const std::string &deviceName);
    void closeSession();

    const PTSIServer &server_;
    int session_id_;
    std::string deviceName_;
    boost::uint64_t pesel_;
    boost::uint16_t samplingFrequency_;
    boost::uint16_t frameSize_;

    friend class PTSIServer;
};

#endif // SESSION_H
