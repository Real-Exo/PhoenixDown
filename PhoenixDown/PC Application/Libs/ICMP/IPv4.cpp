#include <winsock2.h>
#include "Libs/ICMP/IPv4.h"
#include "Libs/Exception/Exception.h"

namespace ICMP
{
IPv4::IPv4(const std::string& ipAddr) :
    mIpAddr{ inet_addr(ipAddr.c_str()) }
{
    if (mIpAddr == INADDR_NONE || mIpAddr == INADDR_ANY)
    {
        throw Exception::RuntimeError{ IPv4Error::conversionFailed, ipAddr };
    }
}

unsigned long
IPv4::operator * () const
{
    return mIpAddr;
}
}
