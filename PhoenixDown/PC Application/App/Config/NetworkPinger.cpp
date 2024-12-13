#include <regex>
#include "App/Config/NetworkPinger.h"
#include "App/Config/Map.h"
#include "App/Config/GreaterThenZeroValidator.h"

namespace
{
bool
IpValidator(const std::string& ip)
{
    return std::regex_match(ip, std::regex{ "^(?=\\d+\\.\\d+\\.\\d+\\.\\d+$)(?:(?:25[0-5]|2[0"
                                            "-4][0-9]|1[0-9]{2}|[1-9][0-9]|[0-9])\\.?){4}$" });
}
}

namespace Config
{
NetworkPinger::NetworkPinger(const Map& configMap) :
    serverIp{ configMap.ExtractValue<std::string>("ServerIP", IpValidator) },
    topPrinterIp{ configMap.ExtractValue<std::string>("TopPrinterIP", IpValidator) },
    bottomPrinterIp{ configMap.ExtractValue<std::string>("BottomPrinterIP", IpValidator) },
    cycleTime{ configMap.ExtractValue<int>("PingCycleTime", greaterThenZeroValidator) },
    timeout{ configMap.ExtractValue<int>("PingTimeout", greaterThenZeroValidator) },
    failuresRequired{ configMap.ExtractValue<int>("PingFailuresRequired", greaterThenZeroValidator) }
{
}
}
