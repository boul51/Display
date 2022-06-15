#ifndef NBOUL_DISPLAY_DISPLAYDRIVER_H
#define NBOUL_DISPLAY_DISPLAYDRIVER_H

#include "color/color.h"
#include "optional.h"
#include "geometry/coordinate.h"
#include "geometry/point.h"
#include "geometry/line.h"
#include "geometry/rectangle.h"

namespace nboul {
namespace display {

class DisplayDriver {
public:
    DisplayDriver(geometry::Coordinate width, geometry::Coordinate height);
    virtual void drawBackground() = 0;
    void setBackgroundColor(const Color& color);

    virtual void drawPoint(const geometry::Point& point, const Color& color) = 0;
    virtual void drawLine(const geometry::Line& line, const Color& color) = 0;
    virtual void drawRectangle(const geometry::Rectangle& rectangle, geometry::Coordinate radius,
                               const nonstd::optional<Color>& strokeColorOpt, const nonstd::optional<Color>& fillColorOpt) = 0;

    //void drawDrawable(const drawable::Drawable& drawable, const geometry::Point& position = geometry::Point{});
    //void eraseDrawable(const drawable::Drawable& drawable, const geometry::Point& position = geometry::Point{});

    geometry::Coordinate width() const { return m_width; }
    geometry::Coordinate height() const { return m_height; }

protected:
    geometry::Coordinate m_width = 0;
    geometry::Coordinate m_height = 0;

    Color m_backgroundColor = colors::White;
};

}  // namespace display
}  // namespace nboul

#endif  // NBOUL_DISPLAY_DISPLAYDRIVER_H
