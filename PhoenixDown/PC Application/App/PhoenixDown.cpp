#include <windows.h>
#include "App/Config/Loader.h"
#include "App/Screen/Interface.h"
#include "App/Network/Pinger.h"
#include "App/Network/PingStatus.h"
#include "App/Printer/HardwareInterface.h"
#include "App/Printer/HardwareStatus.h"
#include "App/Printer/Logic.h"
#include "App/Printer/LogicStatus.h"
#include "App/MouseSynth/LeftClick.h"
#include "App/FatalException.h"
#include "Libs/Util/CappedCounter.h"

void
TryMain()
{
    Config::Loader configLoader;
    Screen::Interface interfc{ configLoader.GetNetworkPinger(),
                               configLoader.GetPrinterHardwareInterface() };
    Network::Pinger pinger { configLoader.GetNetworkPinger() };
    Printer::HardwareInterface hardwareInterface{ configLoader.GetPrinterHardwareInterface() };
    MouseSynth::LeftClick reprintClick{ configLoader.GetMouseSynthLeftClick() };

    Util::CappedCounter<int> topPrinterReprints;
    Util::CappedCounter<int> bottomPrinterReprints;

    while (true)
    {
        const auto serverPingStatus{ pinger.GetServerStatus() };
        const auto topPrinterPingStatus{ pinger.GetTopPrinterStatus() };
        const auto bottomPrinterPingStatus{ pinger.GetBottomPrinterStatus() };

        const auto topPrinterHwStatus{ hardwareInterface.GetTopPrinterStatus() };
        const auto bottomPrinterHwStatus{ hardwareInterface.GetBottomPrinterStatus() };

        const auto topPrinterLogicStatus{ Printer::RunLogic(topPrinterHwStatus,
                                                            bottomPrinterHwStatus,
                                                            serverPingStatus.online,
                                                            topPrinterPingStatus.online) };
        const auto bottomPrinterLogicStatus{ Printer::RunLogic(bottomPrinterHwStatus,
                                                               topPrinterHwStatus,
                                                               serverPingStatus.online,
                                                               bottomPrinterPingStatus.online) };

        if (topPrinterLogicStatus.rebootRequest)
        {
            hardwareInterface.RebootTopPrinter();
        }
        if (bottomPrinterLogicStatus.rebootRequest)
        {
            hardwareInterface.RebootBottomPrinter();
        }

        if (topPrinterLogicStatus.reprintRequest)
        {
            reprintClick();
            ++topPrinterReprints;
        }
        if (bottomPrinterLogicStatus.reprintRequest)
        {
            reprintClick();
            ++bottomPrinterReprints;
        }

        interfc.PrintNetworkHeader();
        interfc.PrintServerPingStatus(serverPingStatus);
        interfc.PrintTopPrinterPingStatus(topPrinterPingStatus);
        interfc.PrintBottomPrinterPingStatus(bottomPrinterPingStatus);

        interfc.PrintHardwareHeader();
        interfc.PrintTopPrinterHwStatus(topPrinterHwStatus, topPrinterLogicStatus,
                                        serverPingStatus.online);
        interfc.PrintReprints(*topPrinterReprints);
        interfc.PrintBottomPrinterHwStatus(bottomPrinterHwStatus, bottomPrinterLogicStatus,
                                           serverPingStatus.online);
        interfc.PrintReprints(*bottomPrinterReprints);

        interfc.Heartbeat();
        interfc.TerminateScreen();

        Sleep(configLoader.GetNetworkPinger().cycleTime);
    }
}

int
main()
{
    try
    {
        TryMain();
    }
    catch(...)
    {
        FatalException();

        return 1;
    }

    return 0;
}
