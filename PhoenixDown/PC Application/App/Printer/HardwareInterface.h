#ifndef HARDWAREINTERFACE_H
#define HARDWAREINTERFACE_H

#include <optional>
#include <vector>
#include <cstdint>
#include "Libs/Serial/ComPort.h"

namespace Config
{
struct PrinterHardwareInterface;
}

namespace Printer
{
struct HardwareStatus;

class HardwareInterface
{
public:

    explicit
    HardwareInterface(const Config::PrinterHardwareInterface& config);
    HardwareInterface(const HardwareInterface&) = delete;
    HardwareInterface(HardwareInterface&&) = default;
    HardwareInterface& operator = (const HardwareInterface&) = delete;
    HardwareInterface& operator = (HardwareInterface&&) = default;

    std::optional<HardwareStatus>
    GetTopPrinterStatus();

    std::optional<HardwareStatus>
    GetBottomPrinterStatus();

    bool
    RebootTopPrinter();

    bool
    RebootBottomPrinter();

private:

    bool
    SendCommand(uint8_t command, int responseLength);

    std::optional<HardwareStatus>
    GetPrinterStatus(uint8_t command);

    Serial::ComPort mComPort;
    std::vector<uint8_t> mDataToSend;
    std::vector<uint8_t> mDataReceived;
};
}

#endif //HARDWAREINTERFACE_H
