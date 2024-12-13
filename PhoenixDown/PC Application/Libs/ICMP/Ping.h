#ifndef PING_H
#define PING_H

namespace ICMP
{
struct PingResponse;
class File;
class IPv4;

PingResponse
Ping(const File& file, const IPv4& ip, int timeout);
}

#endif //PING_H
