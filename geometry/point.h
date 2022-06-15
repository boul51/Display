#ifndef NBOUL_DISPLAY_GEOMETRY_POINT_H
#define NBOUL_DISPLAY_GEOMETRY_POINT_H

#include "geometry/coordinate.h"

namespace nboul {
namespace display {
namespace geometry {

class Point
{
public:
    Coordinate x = 0;
    Coordinate y = 0;

    Point()
        : x(0)
        , y(0)
    {
    }

    Point(const Point& other)
        : x(other.x)
        , y(other.y)
    {
    }

    Point(Coordinate x, Coordinate y)
        : x(x)
        , y(y)
    {
    }

    Point(int x, int y)
        : x{static_cast<Coordinate>(x)}
        , y{static_cast<Coordinate>(y)}
    {
    }

    bool operator == (const Point& other) const {
        return x == other.x && y == other.y;
    }

    Point operator + (const Point& other) const {
        return Point{x + other.x, y + other.y};
    }

    Point operator - (const Point& other) const {
        return Point{x - other.x, y - other.y};
    }

    Point operator % (const Point& other) const {
        return Point{x % other.x, y % other.y};
    }
};

}  // namespace geometry
}  // namespace display
}  // namespace nboul

#endif  // NBOUL_DISPLAY_GEOMETRY_POINT_H
