#include "App/Network/Pinger.h"
#include "App/Config/NetworkPinger.h"
#include "Libs/ICMP/Ping.h"
#include "Libs/ICMP/PingResponse.h"

namespace Network
{
Pinger::Pinger(const Config::NetworkPinger& config) :
    mServerIp{ config.serverIp },
    mTopPrinterIp{ config.topPrinterIp },
    mBottomPrinterIp{ config.bottomPrinterIp },
    mTimeout{ config.timeout },
    mFailuresRequired{ config.failuresRequired }
{
}

PingStatus
Pinger::GetServerStatus()
{
    return GetStatus(mServerIp, mServerStatus);
}

PingStatus
Pinger::GetTopPrinterStatus()
{
    return GetStatus(mTopPrinterIp, mTopPrinterStatus);
}

PingStatus
Pinger::GetBottomPrinterStatus()
{
    return GetStatus(mBottomPrinterIp, mBottomPrinterStatus);
}

PingStatus
Pinger::GetStatus(ICMP::IPv4 ip, PingStatus& status)
{
    const auto pingResponse{ ICMP::Ping(mIcmpFile, ip, mTimeout) };

    status.online = true;
    status.time = pingResponse.time;

    if (pingResponse.success)
    {
        status.failures = 0;
        return status;
    }

    if (++status.failures >= mFailuresRequired)
    {
        status.online = false;
        status.failures = mFailuresRequired;
    }

    return status;
}
}
