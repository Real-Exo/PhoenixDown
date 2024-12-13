#include <regex>
#include "App/Config/PrinterHardwareInterface.h"
#include "App/Config/Map.h"
#include "App/Config/GreaterThenZeroValidator.h"

namespace
{
bool
ComPortValidator(const std::string& portName)
{
    return std::regex_match(portName, std::regex{ "^COM[0-9]+$" });
}
}

namespace Config
{
PrinterHardwareInterface::PrinterHardwareInterface(const Map& configMap) :
    comPortName{ configMap.ExtractValue<std::string>("ComPort", ComPortValidator) },
    baudRate{ configMap.ExtractValue<int>("BaudRate", greaterThenZeroValidator) }
{
}
}
