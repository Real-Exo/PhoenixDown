#ifndef FILE_H
#define FILE_H

#include <windows.h>
#include "Libs/Util/UniqueResource.h"

namespace ICMP
{
enum class FileError
{
    createFailed
};

class File
{
public:

    File();
    File(const File&) = delete;
    File(File&&) = default;
    File& operator = (const File&) = delete;
    File& operator = (File&&) = default;

    HANDLE
    operator * () const;

private:

    Util::UniqueResource<HANDLE> mHandle;
};
}

#endif //FILE_H
