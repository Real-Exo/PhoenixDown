#ifndef COMPORT_H
#define COMPORT_H

#include <string>
#include <cstdint>
#include <windows.h>
#include "Libs/Util/UniqueResource.h"

namespace Serial
{
enum class ComPortError
{
    openFailed,
    configureFailed,
    writeFailed,
    readFailed
};

class ComPort
{
public:

    ComPort(const std::string& name, int baudRate);
    ComPort(const ComPort&) = delete;
    ComPort(ComPort&&) = default;
    ComPort& operator = (const ComPort&) = delete;
    ComPort& operator = (ComPort&&) = default;

    std::string
    GetName() const;

    void
    Write(uint8_t byte) const;

    uint8_t
    Read() const;

private:

    std::string mName;
    Util::UniqueResource<HANDLE> mHandle;
};
}

#endif //COMPORT_H
