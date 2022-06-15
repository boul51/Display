#ifndef NBOUL_DISPLAY_GEOMETRY_OPERATIONS_RECTANGLE_H
#define NBOUL_DISPLAY_GEOMETRY_OPERATIONS_RECTANGLE_H

#include "geometry/rectangle.h"
#include "optional.h"

namespace nboul {
namespace display {
namespace geometry {
namespace operations {

class Rectangle {
public:
    static bool rectanglesIntersect(const geometry::Rectangle& r0, const geometry::Rectangle& r1)
    {
        // If one rectangle is on left side of other
        if (r0.p0.x > r1.p1.x || r1.p0.x > r0.p1.x)
            return false;

        // If one rectangle is above other
        if (r0.p0.y > r1.p1.y || r1.p0.y > r0.p1.y)
            return false;

        return true;
    }

    static nonstd::optional<geometry::Rectangle> rectanglesIntersectionOpt(const geometry::Rectangle& r0, const geometry::Rectangle& r1) {
        if (!rectanglesIntersect(r0, r1))
            return nonstd::nullopt;
        return geometry::Rectangle{{std::max(r0.p0.x, r1.p0.x), std::max(r0.p0.y, r1.p0.y)},
            {std::min(r0.p1.x, r1.p1.x), std::min(r0.p1.y, r1.p1.y)}};
    }

    static bool containsPoint(const geometry::Rectangle& r, const geometry::Point& p)
    {
        return p.x >= r.p0.x && p.x <= r.p1.x && p.y >= r.p0.y && p.y <= r.p1.y;
    }
};

}  // namespace operations
}  // namespace geometry
}  // namespace display
}  // namespace nboul

#endif  // NBOUL_DISPLAY_GEOMETRY_OPERATIONS_RECTANGLE_H
