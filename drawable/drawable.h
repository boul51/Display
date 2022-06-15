#ifndef NBOUL_DISPLAY_DRAWABLE_DRAWABLE_H
#define NBOUL_DISPLAY_DRAWABLE_DRAWABLE_H

#include "geometry/point.h"
#include "geometry/rectangle.h"
#include "displaydriver.h"

#include <vector>

namespace nboul {
namespace display {
namespace drawable {

class Drawable
{
public:
    Drawable(const geometry::Point& position)
        : position{position}
    {
    }

    virtual ~Drawable()
    {
    }

    virtual void draw(const geometry::Rectangle& rect, DisplayDriver& driver) = 0;

    virtual void move(const geometry::Point&) = 0;

    geometry::Point position;
    std::vector<geometry::Rectangle> oldRects;
    std::vector<geometry::Rectangle> newRects;
};

}  // namespace drawable
}  // namespace display
}  // namespace nboul

#endif  // NBOUL_DISPLAY_DRAWABLE_DRAWABLE_H
