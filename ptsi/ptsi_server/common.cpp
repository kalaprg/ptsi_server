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
