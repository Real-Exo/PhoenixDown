#include "App/Config/MouseSynthLeftClick.h"
#include "App/Config/Map.h"
#include "App/Config/GreaterThenZeroValidator.h"

namespace
{
Util::Rectangle<int>
ExtractScreenResolution(const Config::Map& configMap)
{
    const auto width{ configMap.ExtractValue<int>("HorizontalRes",
                                                  Config::greaterThenZeroValidator) };
    const auto height{ configMap.ExtractValue<int>("VerticalRes",
                                                   Config::greaterThenZeroValidator) };

    return{ width, height };
}

template <class XValidator, class YValidator>
Util::Point<int>
ExtractPosition(const Config::Map& configMap,
                const XValidator& xValidator, const YValidator& yValidator)
{
    const auto x{ configMap.ExtractValue<int>("ClickPositionX", xValidator) };
    const auto y{ configMap.ExtractValue<int>("ClickPositionY", yValidator) };

    return{ x, y };
}
}

namespace Config
{
MouseSynthLeftClick::MouseSynthLeftClick(const Map& configMap) :
    screenResolution{ ExtractScreenResolution(configMap) },
    position{ ExtractPosition(configMap,
                              [=](int i){ return (i >= 0 && i <= screenResolution.width); },
                              [=](int i){ return (i >= 0 && i <= screenResolution.height); }) },
    delay{ configMap.ExtractValue<int>("ClickDelay", greaterThenZeroValidator) }
{
}
}
