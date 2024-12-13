#include "App/Config/Loader.h"
#include "App/Config/Map.h"

namespace
{
std::tuple<Config::PrinterHardwareInterface, Config::NetworkPinger, Config::MouseSynthLeftClick>
Load()
{
    Config::Map map;

    return{ Config::PrinterHardwareInterface{ map },
            Config::NetworkPinger{ map },
            Config::MouseSynthLeftClick{ map } };
}
}

namespace Config
{
Loader::Loader() :
    Loader{ Load() }
{
}

PrinterHardwareInterface
Loader::GetPrinterHardwareInterface() const
{
    return mPrinterHardwareInterface;
}

NetworkPinger
Loader::GetNetworkPinger() const
{
    return mNetworkPinger;
}

MouseSynthLeftClick
Loader::GetMouseSynthLeftClick() const
{
    return mMouseSynthLeftClick;
}

Loader::Loader(std::tuple<PrinterHardwareInterface,
               NetworkPinger, MouseSynthLeftClick> config) :
    mPrinterHardwareInterface{ std::get<0>(config) },
    mNetworkPinger{ std::get<1>(config) },
    mMouseSynthLeftClick{ std::get<2>(config) }
{
}
}
