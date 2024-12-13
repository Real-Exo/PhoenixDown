#ifndef MOUSESYNTHLEFTCLICK_H
#define MOUSESYNTHLEFTCLICK_H

#include "Libs/Util/Rectangle.h"
#include "Libs/Util/Point.h"

namespace Config
{
class Map;

struct MouseSynthLeftClick
{
    MouseSynthLeftClick(const Map& configMap);

    Util::Rectangle<int> screenResolution;
    Util::Point<int> position;
    int delay;
};
}

#endif //MOUSESYNTHLEFTCLICK_H
