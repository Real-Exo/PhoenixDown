#ifndef LOADER_H
#define LOADER_H

#include <tuple>
#include "App/Config/PrinterHardwareInterface.h"
#include "App/Config/NetworkPinger.h"
#include "App/Config/MouseSynthLeftClick.h"

namespace Config
{
class Loader
{
public:

    Loader();

    PrinterHardwareInterface
    GetPrinterHardwareInterface() const;

    NetworkPinger
    GetNetworkPinger() const;

    MouseSynthLeftClick
    GetMouseSynthLeftClick() const;

private:

    Loader(std::tuple<PrinterHardwareInterface, NetworkPinger, MouseSynthLeftClick> config);

    PrinterHardwareInterface mPrinterHardwareInterface;
    NetworkPinger mNetworkPinger;
    MouseSynthLeftClick mMouseSynthLeftClick;
};
}

#endif //LOADER_H
