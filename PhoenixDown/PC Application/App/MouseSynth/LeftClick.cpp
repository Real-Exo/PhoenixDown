#include "App/MouseSynth/LeftClick.h"
#include "App/Config/MouseSynthLeftClick.h"
#include "Libs/Exception/Exception.h"
#include "Libs/Util/Narrow.h"

namespace
{
Util::Point<DWORD>
ConvertPosition(const Config::MouseSynthLeftClick& config)
{
    if (config.screenResolution.width <= 0 || config.screenResolution.height <= 0)
    {
        throw Exception::RuntimeError{ MouseSynth::LeftClickError::invalidConfig,
                                       "screenResolution negative or zero!" };
    }

    if (config.position.x < 0 || config.position.x > config.screenResolution.width ||
        config.position.y < 0 || config.position.y > config.screenResolution.height )
    {
        throw Exception::RuntimeError{ MouseSynth::LeftClickError::invalidConfig,
                                       "position outside of screenResolution!" };
    }

    constexpr float maxUnits{ 0xFFFF };
    const auto widthUnitsPerPixel{ maxUnits / config.screenResolution.width };
    const auto heightUnitsPerPixel{ maxUnits / config.screenResolution.height };
    const auto positionX{ widthUnitsPerPixel * config.position.x };
    const auto positionY{ heightUnitsPerPixel * config.position.y };
    const Util::Point<DWORD> position{ Util::NarrowCast<DWORD>(positionX),
                                       Util::NarrowCast<DWORD>(positionY) };

    return position;
}
}

namespace MouseSynth
{
LeftClick::LeftClick(const Config::MouseSynthLeftClick& config) :
    mPosition{ ConvertPosition(config) },
    mClickDelay{ config.delay }
{
}

void
LeftClick::operator()() const
{
    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, mPosition.x, mPosition.y, 0, 0);

    /* Delay between moving the mouse and left clicking. Gives time for any screensaver or
     * monitor power saving to disengage before the click occurs. Without this the click
     * does not register. */
    Sleep(mClickDelay);

    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, mPosition.x, mPosition.y, 0, 0);
    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, mPosition.x, mPosition.y, 0, 0);
}
}
