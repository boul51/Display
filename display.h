#ifndef NBOUL_DISPLAY_DISPLAY_H
#define NBOUL_DISPLAY_DISPLAY_H

#include "displaydriver.h"
#include "color/color.h"
#include "drawable/grid.h"
#include "geometry/point.h"

namespace nboul {
namespace display {

class DisplayDriver;

class Display
{
public:
    Display(DisplayDriver* driver);
    void setBackgroundColor(const Color& color);
    void setGrid(const drawable::Grid& grid, const geometry::Point& position);
    const DisplayDriver* driverConst() const { return m_driver; }
    DisplayDriver* driver() { return m_driver; }
    void drawGrid();

private:
    DisplayDriver* m_driver = nullptr;
    drawable::Grid* m_grid = nullptr;
    geometry::Point m_gridPosition;
};

}  // namespace display
}  // namespace nboul

#endif  // NBOUL_DISPLAY_DISPLAY_H
