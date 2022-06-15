#ifndef NBOUL_DISPLAY_GEOMETRY_RECTANGLE_H
#define NBOUL_DISPLAY_GEOMETRY_RECTANGLE_H

#include "geometry/coordinate.h"
#include "geometry/point.h"
#include "geometry/line.h"

#include "optional.h"

namespace nboul {
namespace display {
namespace geometry {

class Rectangle
{
public:
    geometry::Point p0;
    geometry::Point p1;

    Rectangle(const geometry::Point& p0, const geometry::Point& p1)
        : p0{std::min(p0.x, p1.x), std::min(p0.y, p1.y)}
        , p1{std::max(p0.x, p1.x), std::max(p0.y, p1.y)}
    {
    }

    bool operator == (const Rectangle& other) const
    {
        return p0 == other.p0 && p1 == other.p1;
    }
};

}  // namespace geometry
}  // namespace display
}  // namespace nboul

#endif  // NBOUL_DISPLAY_GEOMETRY_RECTANGLE_H
