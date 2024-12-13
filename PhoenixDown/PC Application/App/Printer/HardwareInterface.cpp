#include <string>
#include "App/Printer/HardwareInterface.h"
#include "App/Printer/HardwareStatus.h"
#include "App/Config/PrinterHardwareInterface.h"
#include "Libs/Serial/Protocol.h"
#include "Libs/Exception/Exception.h"

/* Commands:
 * 0x01 - Identify - Hardware should respond with "Phnx" (not 0-terminated).
 * 0x10 - Get top printer status byte.
 * 0x15 - Get bottom printer status byte.
 *        bit<0> - power
 *        bit<1> - ready
 *        bit<2> - reboots inhibited
 *        bit<3> - reprint request
 * 0x20 - Reboot top printer.
 * 0x25 - Reboot bottom printer. */

namespace
{
bool
operator != (const std::vector<uint8_t>& lhs, const std::string& rhs)
{
    const std::vector<uint8_t> v{ rhs.begin(), rhs.end() };

    return lhs != v;
}

void
FilterExceptions()
{
    try
    {
        throw;
    }
    catch(const Exception::RuntimeError<Serial::ComPortError>&)
    {
        return;
    }
    catch(const Exception::RuntimeError<Serial::ProtocolError>&)
    {
        return;
    }
}

Printer::HardwareStatus
DecodeHardwareStatus(uint8_t byte)
{
    constexpr uint8_t powerMask{ 0x01 };
    constexpr uint8_t readyMask{ 0x02 };
    constexpr uint8_t inhibitMask{ 0x04 };
    constexpr uint8_t reprintRequestMask{ 0x08 };

    Printer::HardwareStatus hardwareStatus{};
    hardwareStatus.power = byte & powerMask;
    hardwareStatus.ready = byte & readyMask;
    hardwareStatus.inhibit = byte & inhibitMask;
    hardwareStatus.reprintRequest = byte & reprintRequestMask;

    return hardwareStatus;
}
}

namespace Printer
{
HardwareInterface::HardwareInterface(const Config::PrinterHardwareInterface& config) :
    mComPort{ config.comPortName, config.baudRate }
{
}

std::optional<HardwareStatus>
HardwareInterface::GetTopPrinterStatus()
{
    constexpr uint8_t command{ 0x10 };

    return GetPrinterStatus(command);
}

std::optional<HardwareStatus>
HardwareInterface::GetBottomPrinterStatus()
{
    constexpr uint8_t command{ 0x15 };

    return GetPrinterStatus(command);
}

bool
HardwareInterface::RebootTopPrinter()
{
    constexpr uint8_t command{ 0x20 };
    constexpr auto responseLength{ 0 };

    return SendCommand(command, responseLength);
}

bool
HardwareInterface::RebootBottomPrinter()
{
    constexpr uint8_t command{ 0x25 };
    constexpr auto responseLength{ 0 };

    return SendCommand(command, responseLength);
}

bool
HardwareInterface::SendCommand(uint8_t command, int responseLength)
{
    try
    {
        {
            //Identify before every command, make sure we're not talking to strangers.
            constexpr uint8_t idCommand{ 0x01 };
            const std::string id{ "Phnx" };

            mDataToSend.clear();
            mDataToSend.push_back(idCommand);
            Serial::SendCommand(mComPort, mDataToSend, mDataReceived, id.size());

            if (mDataReceived != id)
            {
                return false;
            }
        }

        mDataToSend.clear();
        mDataToSend.push_back(command);
        Serial::SendCommand(mComPort, mDataToSend, mDataReceived, responseLength);

        return true;
    }
    catch(...)
    {
        FilterExceptions();

        return false;
    }
}

std::optional<HardwareStatus>
HardwareInterface::GetPrinterStatus(uint8_t command)
{
    constexpr auto responseLength{ 1 };

    if (SendCommand(command, responseLength))
    {
        return DecodeHardwareStatus(mDataReceived[0]);
    }

    return std::nullopt;
}
}
