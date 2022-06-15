#ifndef NBOUL_DISPLAY_DRAWABLE_RECTANGLE_H
#define NBOUL_DISPLAY_DRAWABLE_RECTANGLE_H

#include "drawable/drawable.h"
#include "geometry/coordinate.h"
#include "color/color.h"
#include "geometry/operations/rectangle.h"

#include "optional.h"

namespace nboul {
namespace display {
namespace drawable {

struct Rectangle : public Drawable {

public:

    Rectangle(const geometry::Point& p0, const geometry::Point& p1, geometry::Coordinate radius, const ColorOpt& strokeColorOpt, const ColorOpt& fillColorOpt)
        : Drawable{p0}
        , p1{p1}
        , radius{radius}
        , strokeColorOpt{strokeColorOpt}
        , fillColorOpt{fillColorOpt}
    {
        newRects.push_back({position, p1});
    }

    geometry::Point p1;
    geometry::Coordinate radius;

    ColorOpt strokeColorOpt;
    ColorOpt fillColorOpt;

    void draw(const geometry::Rectangle& rect, DisplayDriver& driver) override
    {
        if (const auto& intersectionOpt = geometry::operations::Rectangle::rectanglesIntersectionOpt({position, p1}, rect))
            driver.drawRectangle(*intersectionOpt, 0, strokeColorOpt, fillColorOpt);
    }

    void move(const geometry::Point &position) override
    {
        p1 = p1 + position - this->position;
        this->position = position;
        newRects = {{position, p1}};
    }

private:
};

}  // namespace drawable
}  // namespace display
}  // namespace nboul

#endif  // NBOUL_DISPLAY_RECTANGLE_H
