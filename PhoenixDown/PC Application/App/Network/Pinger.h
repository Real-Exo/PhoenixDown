#ifndef PINGER_H
#define PINGER_H

#include "Libs/ICMP/File.h"
#include "Libs/ICMP/IPv4.h"
#include "App/Network/PingStatus.h"

namespace Config
{
struct NetworkPinger;
}

namespace Network
{
class Pinger
{
public:

    explicit
    Pinger(const Config::NetworkPinger& config);
    Pinger(const Pinger&) = delete;
    Pinger(Pinger&&) = default;
    Pinger& operator = (const Pinger&) = delete;
    Pinger& operator = (Pinger&&) = default;

    PingStatus
    GetServerStatus();

    PingStatus
    GetTopPrinterStatus();

    PingStatus
    GetBottomPrinterStatus();

private:

    PingStatus
    GetStatus(ICMP::IPv4 ip, PingStatus& status);

    ICMP::File mIcmpFile;
    ICMP::IPv4 mServerIp;
    ICMP::IPv4 mTopPrinterIp;
    ICMP::IPv4 mBottomPrinterIp;
    int mTimeout;
    int mFailuresRequired;

    PingStatus mServerStatus{};
    PingStatus mTopPrinterStatus{};
    PingStatus mBottomPrinterStatus{};
};
}

#endif //PINGER_H
