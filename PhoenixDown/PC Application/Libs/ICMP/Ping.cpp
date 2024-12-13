#include <iphlpapi.h>
#include <icmpapi.h>
#include "Libs/ICMP/Ping.h"
#include "Libs/ICMP/PingResponse.h"
#include "Libs/ICMP/File.h"
#include "Libs/ICMP/IPv4.h"
#include "Libs/Util/Narrow.h"

namespace ICMP
{
PingResponse
Ping(const File& file, const IPv4& ip, int timeout)
{
    //As per MSDN example...
    constexpr auto requestSize{ 32 };
    constexpr auto replySize{ sizeof(ICMP_ECHO_REPLY) + requestSize };
    char requestBuff[requestSize]{ "Ping?" };
    char replyBuff[replySize];

    const auto result = IcmpSendEcho(*file, *ip, requestBuff, requestSize, NULL,
                                     replyBuff, replySize, Util::Narrow<DWORD>(timeout));

    PingResponse response{};
    if (result)
    {
        const auto reply{ reinterpret_cast<ICMP_ECHO_REPLY*>(replyBuff) };
        IN_ADDR replyIp;
        replyIp.S_un.S_addr = reply->Address;

        response.success = reply->Status == IP_SUCCESS;
        response.time = Util::Narrow<int>(reply->RoundTripTime);
        response.replyIp = inet_ntoa(replyIp);
    }

    return response;
}
}
