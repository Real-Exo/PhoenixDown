#include "Libs/Serial/ComPort.h"
#include "Libs/Exception/Exception.h"
#include "Libs/Util/Narrow.h"

namespace
{
HANDLE
OpenPort(const std::string& name)
{
    const auto handle{ CreateFileA(name.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
                                   NULL, OPEN_EXISTING, 0, NULL) };

    if (handle == INVALID_HANDLE_VALUE)
    {
        throw Exception::RuntimeError{ Serial::ComPortError::openFailed, name };
    }

    return handle;
}

void
SetBufferSizes(const HANDLE& handle)
{
    constexpr DWORD bufferSize{ 256 };

    if (!SetupComm(handle, bufferSize, bufferSize))
    {
        throw Exception::RuntimeError{ Serial::ComPortError::configureFailed, "SetupComm failed!" };
    }
}

void
Configure(const HANDLE& handle, int baudRate)
{
    DCB dcb;
    if (!GetCommState(handle, &dcb))
    {
        throw Exception::RuntimeError{ Serial::ComPortError::configureFailed,
                                       "GetCommState failed!" };
    }

    dcb.BaudRate = Util::Narrow<DWORD>(baudRate);
    dcb.fBinary = true;
    dcb.fParity = false;
    dcb.fOutxCtsFlow = false;
    dcb.fOutxDsrFlow = false;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fDsrSensitivity = false;
    dcb.fTXContinueOnXoff = true;
    dcb.fOutX = false;
    dcb.fInX = false;
    dcb.fNull = false;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fAbortOnError = false;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;

    if (!SetCommState(handle, &dcb))
    {
        throw Exception::RuntimeError{ Serial::ComPortError::configureFailed,
                                       "SetCommState failed!" };
    }
}

void
SetTimeouts(const HANDLE& handle)
{
    constexpr DWORD readTimeout{ 100 };

    COMMTIMEOUTS timeouts;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = readTimeout;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadIntervalTimeout = 0;

    if (!SetCommTimeouts(handle, &timeouts))
    {
        throw Exception::RuntimeError{ Serial::ComPortError::configureFailed,
                                       "SetCommTimeouts failed!" };
    }
}
}

namespace Serial
{
ComPort::ComPort(const std::string& name, int baudRate) :
    mName{ name },
    mHandle{ OpenPort(name), [](HANDLE& h){ CloseHandle(h); } }
{
    SetBufferSizes(*mHandle);
    Configure(*mHandle, baudRate);
    SetTimeouts(*mHandle);
}

std::string
ComPort::GetName() const
{
    return mName;
}

void
ComPort::Write(uint8_t byte) const
{
    DWORD nWritten{ 0 };

    if (!WriteFile(*mHandle, &byte, 1, &nWritten, NULL) || !nWritten)
    {
        throw Exception::RuntimeError{ ComPortError::writeFailed };
    }
}

uint8_t
ComPort::Read() const
{
    uint8_t byte;
    DWORD nRead{ 0 };

    if (!ReadFile(*mHandle, &byte, 1, &nRead, NULL) || !nRead)
    {
        throw Exception::RuntimeError{ ComPortError::readFailed };
    }

    return byte;
}
}
