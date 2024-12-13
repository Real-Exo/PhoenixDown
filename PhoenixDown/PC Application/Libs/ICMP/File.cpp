#include <iphlpapi.h>
#include <icmpapi.h>
#include "Libs/ICMP/File.h"
#include "Libs/Exception/Exception.h"

namespace
{
HANDLE
MakeFile()
{
    const auto handle{ IcmpCreateFile() };

    if (handle == INVALID_HANDLE_VALUE)
    {
        throw Exception::RuntimeError{ ICMP::FileError::createFailed };
    }

    return handle;
}
}

namespace ICMP
{
File::File() :
    mHandle{ MakeFile(), [](HANDLE& h){ IcmpCloseHandle(h); } }
{
}

HANDLE
File::operator * () const
{
    return *mHandle;
}
}


