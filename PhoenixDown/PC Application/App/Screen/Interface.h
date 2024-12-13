#ifndef INTERFACE_H
#define INTERFACE_H

#include <optional>
#include "App/Screen/Console.h"
#include "App/Config/NetworkPinger.h"
#include "App/Config/PrinterHardwareInterface.h"
#include "Libs/Util/CappedCounter.h"

namespace Network
{
struct PingStatus;
}

namespace Printer
{
struct HardwareStatus;
struct LogicStatus;
}

namespace Screen
{
class Interface
{
public:

    Interface(const Config::NetworkPinger& pingerConfig,
              const Config::PrinterHardwareInterface& hardwareInterfaceConfig);
    Interface(const Interface&) = delete;
    Interface(Interface&&) = default;
    Interface& operator = (const Interface&) = delete;
    Interface& operator = (Interface&&) = default;

    void
    PrintNetworkHeader() const;

    void
    PrintServerPingStatus(const Network::PingStatus& pingStatus);

    void
    PrintTopPrinterPingStatus(const Network::PingStatus& pingStatus);

    void
    PrintBottomPrinterPingStatus(const Network::PingStatus& pingStatus);

    void
    PrintHardwareHeader() const;

    void
    PrintTopPrinterHwStatus(const std::optional<Printer::HardwareStatus>& hardwareStatus,
                            const Printer::LogicStatus& logicStatus, bool serverOnline);

    void
    PrintBottomPrinterHwStatus(const std::optional<Printer::HardwareStatus>& hardwareStatus,
                               const Printer::LogicStatus& logicStatus, bool serverOnline);

    void
    PrintReprints(int reprints) const;

    void
    Heartbeat();

    void
    TerminateScreen() const;

private:

    Console mConsole;
    Config::NetworkPinger mPingerConfig;
    Config::PrinterHardwareInterface mHardwareInterfaceConfig;
    Util::CircularCounter<int> mHeartbeatCnt;
};
}

#endif //INTERFACE_H
