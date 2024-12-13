#ifndef NETWORKPINGER_H
#define NETWORKPINGER_H

#include <string>

namespace Config
{
class Map;

struct NetworkPinger
{
    explicit
    NetworkPinger(const Map& configMap);

    std::string serverIp;
    std::string topPrinterIp;
    std::string bottomPrinterIp;
    int cycleTime;
    int timeout;
    int failuresRequired;
};
}

#endif //NETWORKPINGER_H
