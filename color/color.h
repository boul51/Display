#ifndef NBOUL_DISPLAY_DISPLAYCOLOR_H
#define NBOUL_DISPLAY_DISPLAYCOLOR_H

#include "optional.h"

namespace nboul {
namespace display {

struct Color {
    int r;
    int g;
    int b;

    Color(int r, int g, int b)
        : r(r)
        , g(g)
        , b(b)
    {
    }

    Color(const Color& other)
        : r(other.r)
        , g(other.g)
        , b(other.b)
    {
    }

    Color operator*(float mult) const
    {
        return Color{static_cast<int>(r * mult), static_cast<int>(g * mult), static_cast<int>(b * mult)};
    }

    Color lighter(float mult) const
    {
        return *this * mult;
    }

    Color darker(float mult) const
    {
        return *this * (1/mult);
    }

    uint16_t toRgb565() const
    {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }
};

using ColorOpt = nonstd::optional<Color>;

namespace colors {
static const Color Red{255, 0, 0};
static const Color Green{0, 255, 0};
static const Color Blue{0, 0, 255};

static const Color White{255, 255, 255};
static const Color Black{0, 0, 0};

static const Color Yellow{255, 255, 0};
static const Color Cyan{0, 255, 255};
static const Color Magenta{255, 00, 255};

static const ColorOpt None;

}  // namespace colors

}  // namespace display
}  // namespace nboul

#endif  // NBOUL_DISPLAY_DISPLAYCOLOR_H
