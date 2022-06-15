#ifndef NBOUL_DISPLAY_CANVAS_CANVAS_H_
#define NBOUL_DISPLAY_CANVAS_CANVAS_H_

#include "drawable/drawable.h"
#include "geometry/coordinate.h"
#include "displaydriver.h"

#include <vector>
#include <utility>

namespace nboul {
namespace display {
namespace canvas {

class Canvas {
public:
    Canvas(DisplayDriver& driver)
        : driver(driver)
    {
    }

    void addDrawable(drawable::Drawable* drawable, geometry::ZCoordinate z)
    {
        m_drawables.push_back({drawable, z});
        std::sort(m_drawables.begin(), m_drawables.end(), [](const CanvasDrawable& drawable1, const CanvasDrawable& drawable2) {
            return drawable1.z < drawable2.z;
        });
    }

    void draw()
    {
        for (const CanvasDrawable& drawable : m_drawables) {
            for (const auto& oldRect : drawable.drawable->oldRects) {
                for (const CanvasDrawable& innerDrawable : m_drawables) {
                    if (innerDrawable.drawable != drawable.drawable) {
                        innerDrawable.drawable->draw(oldRect, driver);
                    }
                }
            }
        }

        for (const CanvasDrawable& drawable : m_drawables) {
            for (const auto& newRect : drawable.drawable->newRects) {
                drawable.drawable->draw(newRect, driver);
            }
        }

        for (const CanvasDrawable& drawable : m_drawables) {
            drawable.drawable->oldRects.clear();
            drawable.drawable->newRects.clear();
        }
    }

private:
    DisplayDriver& driver;

    struct CanvasDrawable{
        drawable::Drawable* drawable;
        geometry::ZCoordinate z;
    };

    std::vector<CanvasDrawable> m_drawables;
};

}  // namespace nboul
}  // namespace display
}  // namespace canvas

#endif  // NBOUL_DISPLAY_CANVAS_CANVAS_H_
