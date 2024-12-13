#ifndef LOGIC_H
#define LOGIC_H

#include <optional>

namespace Printer
{
class LogicStatus;
class HardwareStatus;

LogicStatus
RunLogic(const std::optional<HardwareStatus>& hardwareStatus,
         const std::optional<HardwareStatus>& otherPrinterHardwareStatus,
         bool ServerOnline, bool PrinterOnline);
}

#endif //LOGIC_H
