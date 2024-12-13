#ifndef IPV4_H
#define IPV4_H

#include <string>

namespace ICMP
{
enum class IPv4Error
{
    conversionFailed
};

class IPv4
{
public:

    explicit
    IPv4(const std::string& ipAddr);

    unsigned long
    operator * () const;

private:

    unsigned long mIpAddr;
};
}

#endif //IPV4_H
