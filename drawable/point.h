#ifndef NBOUL_DISPLAY_DRAWABLE_POINT_H
#define NBOUL_DISPLAY_DRAWABLE_POINT_H

#include "drawable/drawable.h"
#include "geometry/coordinate.h"
#include "color/color.h"
#include "geometry/operations/rectangle.h"

#include "optional.h"

namespace nboul {
namespace display {
namespace drawable {

struct Point : public Drawable {

public:

    Point(const geometry::Point& position, const Color& color)
        : Drawable{position}
        , color{color}
    {
        newRects = {geometry::Rectangle{position, position + geometry::Point{1, 1}}};
    }

    Color color;

    void draw(const geometry::Rectangle& rect, DisplayDriver& driver) override
    {
        if (geometry::operations::Rectangle::containsPoint(rect, position))
            driver.drawPoint(position, color);
    }

    void move(const geometry::Point &position) override
    {
        oldRects = {geometry::Rectangle{this->position, this->position + geometry::Point{1, 1}}};
        this->position = position;
        newRects = {geometry::Rectangle{this->position, this->position + geometry::Point{1, 1}}};
    }

private:
};

}  // namespace drawable
}  // namespace display
}  // namespace nboul

#endif  // NBOUL_DISPLAY_DRAWABLE_POINT_H
