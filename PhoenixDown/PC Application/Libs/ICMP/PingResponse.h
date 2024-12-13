#ifndef PINGRESPONSE_H
#define PINGRESPONSE_H

#include <string>

namespace ICMP
{
struct PingResponse
{
    bool success;
    int time;
    std::string replyIp;
};
}

#endif //PINGRESPONSE_H
