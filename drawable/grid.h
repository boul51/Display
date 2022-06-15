#ifndef NBOUL_DISPLAY_DRAWABLE_GRID_H_
#define NBOUL_DISPLAY_DRAWABLE_GRID_H_

#include "drawable/drawable.h"
#include "geometry/coordinate.h"
#include "geometry/point.h"
#include "color/color.h"

#include <vector>
#include <algorithm>

namespace nboul {
namespace display {
namespace drawable {

class Grid : public Drawable {
public:
    struct Division {
        int main;
        int sub;
    };

    enum LineType {
        Main,
        Sub,
    };

    enum LineOrientation {
        Horizontal,
        Vertical,
    };

    Grid(geometry::Point position, geometry::Coordinate w, geometry::Coordinate h, const Division& hDivision, const Division& vDivision, const Color& mainColor, const Color& subColor)
        : Drawable{position}
        , w{w}
        , h{h}
        , hDivision(hDivision)
        , vDivision(vDivision)
        , mainColor{mainColor}
        , subColor{subColor}
    {
    }

    geometry::Coordinate w;
    geometry::Coordinate h;
    Division hDivision;
    Division vDivision;
    Color mainColor;
    Color subColor;

    void draw(const geometry::Rectangle& rect, display::DisplayDriver& driver) override
    {
        drawLines(rect, LineType::Sub, LineOrientation::Horizontal, driver);
        drawLines(rect, LineType::Sub, LineOrientation::Vertical, driver);
        drawLines(rect, LineType::Main, LineOrientation::Horizontal, driver);
        drawLines(rect, LineType::Main, LineOrientation::Vertical, driver);
    }

    void move(const geometry::Point &position)
    {
    }

private:
    void drawLines(const geometry::Rectangle &rect, LineType type, LineOrientation orientation, display::DisplayDriver& driver)
    {
        const Division& div = (orientation == LineOrientation::Vertical ? hDivision : vDivision);
        const int* const divisions = (type == LineType::Main ? &div.main : &div.sub);
        const int linesCount = *divisions + 1;
        const Color& color = (type == LineType::Main ? mainColor : subColor);

        for (int i = 0; i < linesCount; i++) {
            geometry::Coordinate x0, y0, x1, y1;
            if (orientation == LineOrientation::Horizontal) {
                x0 = position.x;
                x1 = position.x + w - 1;
                y0 = y1 = position.y + (i * h - 1) / *divisions;
            } else {
                y0 = position.y + 0;
                y1 = position.y + h - 1;
                x0 = x1 = position.x + (i * w - 1) / *divisions;
            }

            driver.drawLine({{x0, y0}, {x1, y1}}, color);
        }
    }
};

}  // namespace drawable
}  // namespace display
}  // nboul

#endif  // NBOUL_DISPLAY_DRAWABLE_GRID_H_
