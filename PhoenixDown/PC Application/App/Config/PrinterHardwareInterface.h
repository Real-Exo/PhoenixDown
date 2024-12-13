#ifndef PRINTERHARDWAREINTERFACE_H
#define PRINTERHARDWAREINTERFACE_H

#include <string>

namespace Config
{
class Map;

struct PrinterHardwareInterface
{
    explicit
    PrinterHardwareInterface(const Map& configMap);

    std::string comPortName;
    int baudRate;
};
}

#endif //PRINTERHARDWAREINTERFACE_H
