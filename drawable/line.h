#ifndef NBOUL_DISPLAY_DRAWABLE_LINE_H
#define NBOUL_DISPLAY_DRAWABLE_LINE_H

#include "drawable/drawable.h"
#include "geometry/coordinate.h"
#include "color/color.h"
#include "geometry/operations/rectangle.h"

#include "optional.h"

namespace nboul {
namespace display {
namespace drawable {

struct Line : public Drawable {

public:

    Line(const geometry::Point& p0, const geometry::Point& p1, const Color& color)
        : Drawable{p0}
        , p1{p1}
        , color{color}
    {
        newRects = {boundingRect()};
    }

    void draw(const geometry::Rectangle& rect, DisplayDriver& driver) override
    {
        const geometry::Rectangle& myRect = boundingRect();
        // TODO(nboul): draw only part of the line ?
        if (geometry::operations::Rectangle::rectanglesIntersect(myRect, rect))
            driver.drawLine(geometry::Line{position, p1}, color);
    }

    void move(const geometry::Point &position) override
    {
        oldRects = {boundingRect()};
        p1 = p1 + position - this->position;
        this->position = position;
        newRects = {boundingRect()};
    }

private:
    // TODO(nboul): split into several smaller rectangles
    geometry::Rectangle boundingRect() const {
        return geometry::Rectangle{position, p1 + geometry::Point{1, 1}};
    }

    geometry::Point p1;
    Color color;
};

}  // namespace drawable
}  // namespace display
}  // namespace nboul

#endif  // NBOUL_DISPLAY_DRAWABLE_POINT_H
