#ifndef LEFTCLICK_H
#define LEFTCLICK_H

#include <windows.h>
#include "Libs/Util/Point.h"

namespace Config
{
struct MouseSynthLeftClick;
}

namespace MouseSynth
{
enum class LeftClickError
{
    invalidConfig
};

class LeftClick
{
public:

    explicit
    LeftClick(const Config::MouseSynthLeftClick& config);

    void
    operator()() const;

private:

    Util::Point<DWORD> mPosition;
    int mClickDelay;
};
}

#endif //LEFTCLICK_H
