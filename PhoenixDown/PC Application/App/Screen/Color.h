#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

namespace Screen
{
struct Color
{
    enum : uint8_t
    {
        blue = 0x01,
        green = 0x02,
        red = 0x04,
        intense = 0x08
    };
};
}

#endif //COLOR_H
