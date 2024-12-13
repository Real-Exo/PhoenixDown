#include <string>
#include <iostream>
#include <iomanip>
#include "App/Screen/Interface.h"
#include "App/Screen/Color.h"
#include "App/Network/PingStatus.h"
#include "App/Printer/HardwareStatus.h"
#include "App/Printer/LogicStatus.h"

namespace
{
const std::string title{ "Phoenix Down - Sofidel labeler watchdog - RC1." };
const std::string topPrinterName{ "Top printer" };
const std::string bottomPrinterName{ "Bottom printer" };
constexpr auto okColor{ Screen::Color::green | Screen::Color::intense };
constexpr auto cautionColor{ Screen::Color::green | Screen::Color::red | Screen::Color::intense };
constexpr auto errorColor{ Screen::Color::red | Screen::Color::intense };
constexpr auto whiteColor{ Screen::Color::red | Screen::Color::green |
                           Screen::Color::blue | Screen::Color::intense };
constexpr auto descriptionFieldWidth{ 14 };
constexpr auto ipFieldWidth{ 15 };
constexpr auto heartbeatSteps{ 6 };

void
PrintDescription(const std::string& description)
{
    std::cout << "  " << std::setw(descriptionFieldWidth) << description << " : ";
}

void
PrintPingStatus(Screen::Console& console, const std::string& deviceName,
                const std::string& ip, const Network::PingStatus& pingStatus,
                int failuresRequired)
{
    console.TerminateLine();
    PrintDescription(deviceName);
    std::cout << std::setw(ipFieldWidth) << ip << "  ";

    if (pingStatus.online)
    {
        console.SetForegroundColor(okColor);
        std::cout << "- ONLINE  - ";
    }
    else
    {
        console.SetForegroundColor(errorColor);
        std::cout << "- OFFLINE - ";
    }
    console.RestoreOriginalTextAttributes();

    if (pingStatus.failures && pingStatus.failures < failuresRequired)
    {
        std::cout << pingStatus.failures << "/" << failuresRequired << " failures.";
    }
    if (!pingStatus.failures)
    {
        std::cout << "Time: " << pingStatus.time << " ms.";
    }
}

void
PrintHardwareStatus(Screen::Console& console, const std::string& deviceName,
                    const std::optional<Printer::HardwareStatus>& hardwareStatus,
                    const Printer::LogicStatus& logicStatus, bool serverOnline)
{
    console.TerminateLine();
    PrintDescription(deviceName);

    if (logicStatus.rebootRequest)
    {
        console.SetForegroundColor(errorColor);
        std::cout << "- REBOOTING - ";
    }
    else
    if (logicStatus.ignored)
    {
        console.SetForegroundColor(cautionColor);
        std::cout << "-  IGNORED  - ";
    }
    else
    {
        console.SetForegroundColor(okColor);
        std::cout << "- GUARDING  - ";
    }
    console.RestoreOriginalTextAttributes();

    if (!hardwareStatus)
    {
        std::cout << "(Communications failure)";

        return;
    }

    if (!serverOnline)
    {
        std::cout << "(Server offline) ";
    }

    std::cout << (hardwareStatus->power ? "Powered on" : "Powered off");
    std::cout << (hardwareStatus->ready ? ", ready" : ", not ready");
    std::cout << (hardwareStatus->inhibit ? ", inhibit" : "");
}
}

namespace Screen
{
Interface::Interface(const Config::NetworkPinger& pingerConfig,
                     const Config::PrinterHardwareInterface& hardwareInterfaceConfig) :
    mPingerConfig{ pingerConfig },
    mHardwareInterfaceConfig{ hardwareInterfaceConfig },
    mHeartbeatCnt{ 0, heartbeatSteps }
{
    mConsole.SetTitle(title);
}

void
Interface::PrintNetworkHeader() const
{
    mConsole.TerminateLine();
    std::cout << "  Network";
    mConsole.TerminateLine();
    std::cout << "  -------";
    mConsole.TerminateLine();
    std::cout << "  Running ping cycle every " << mPingerConfig.cycleTime << " ms.";
    mConsole.TerminateLine();
    std::cout << "  " << mPingerConfig.failuresRequired
              << " successive ping failures required before device considered offline.";
    mConsole.TerminateLine();
}

void
Interface::PrintServerPingStatus(const Network::PingStatus& pingStatus)
{
    PrintPingStatus(mConsole, "Server", mPingerConfig.serverIp,
                    pingStatus, mPingerConfig.failuresRequired);
}

void
Interface::PrintTopPrinterPingStatus(const Network::PingStatus& pingStatus)
{
    PrintPingStatus(mConsole, topPrinterName, mPingerConfig.topPrinterIp,
                    pingStatus, mPingerConfig.failuresRequired);
}

void
Interface::PrintBottomPrinterPingStatus(const Network::PingStatus& pingStatus)
{
    PrintPingStatus(mConsole, bottomPrinterName, mPingerConfig.bottomPrinterIp,
                    pingStatus, mPingerConfig.failuresRequired);
}

void
Interface::PrintHardwareHeader() const
{
    mConsole.TerminateLine();
    mConsole.TerminateLine();
    std::cout << "  Hardware";
    mConsole.TerminateLine();
    std::cout << "  --------";
    mConsole.TerminateLine();
    std::cout << "  Working on " << mHardwareInterfaceConfig.comPortName
              << " with " << mHardwareInterfaceConfig.baudRate << " baud.";
    mConsole.TerminateLine();
}

void
Interface::PrintTopPrinterHwStatus(const std::optional<Printer::HardwareStatus>& hardwareStatus,
                                   const Printer::LogicStatus& logicStatus, bool serverOnline)
{
    PrintHardwareStatus(mConsole, topPrinterName, hardwareStatus, logicStatus, serverOnline);
}

void
Interface::PrintBottomPrinterHwStatus(const std::optional<Printer::HardwareStatus>& hardwareStatus,
                                      const Printer::LogicStatus& logicStatus, bool serverOnline)
{
    PrintHardwareStatus(mConsole, bottomPrinterName, hardwareStatus, logicStatus, serverOnline);
}

void
Interface::PrintReprints(int reprints) const
{
    mConsole.TerminateLine();
    PrintDescription("Interventions");
    std::cout << reprints;
}

void
Interface::Heartbeat()
{
    mConsole.TerminateLine();
    mConsole.TerminateLine();
    PrintDescription("Heartbeat");

    ++mHeartbeatCnt;

    switch(*mHeartbeatCnt)
    {
    case 1:
    case 5: mConsole.SetForegroundColor(Screen::Color::intense);    break;
    case 2:
    case 4: mConsole.SetForegroundColor(whiteColor);    break;
    case 3: mConsole.SetForegroundColor(errorColor);    break;
    default: break;
    }

    if (*mHeartbeatCnt)
    {
        std::cout << '\x03';
    }

    mConsole.RestoreOriginalTextAttributes();
    mConsole.TerminateLine();
}

void
Interface::TerminateScreen() const
{
    mConsole.TerminateScreen();
}
}
