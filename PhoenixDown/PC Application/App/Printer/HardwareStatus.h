#ifndef HARDWARESTATUS_H
#define HARDWARESTATUS_H

namespace Printer
{
struct HardwareStatus
{
    bool power;
    bool ready;
    bool inhibit;
    bool reprintRequest;
};
}

#endif //HARDWARESTATUS_H
