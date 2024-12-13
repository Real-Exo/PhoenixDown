#include "App/Printer/Logic.h"
#include "App/Printer/LogicStatus.h"
#include "App/Printer/HardwareStatus.h"

namespace Printer
{
LogicStatus
RunLogic(const std::optional<HardwareStatus>& hardwareStatus,
         const std::optional<HardwareStatus>& otherPrinterHardwareStatus,
         bool serverOnline, bool printerOnline)
{
    LogicStatus logicStatus{};

    if (!hardwareStatus)
    {
        logicStatus.ignored = true;
        logicStatus.rebootRequest = false;
        logicStatus.reprintRequest = false;

        return logicStatus;
    }

    logicStatus.ignored = !serverOnline || !hardwareStatus->power || hardwareStatus->inhibit;

    if (hardwareStatus->power)
    {
        logicStatus.rebootRequest = false;
    }
    if (!logicStatus.ignored && !printerOnline)
    {
        logicStatus.rebootRequest = true;
    }

    if (!otherPrinterHardwareStatus)
    {
        logicStatus.reprintRequest = false;

        return logicStatus;
    }
    logicStatus.reprintRequest = hardwareStatus->reprintRequest && hardwareStatus->ready &&
                                 otherPrinterHardwareStatus->ready;

    return logicStatus;
}
}
