#include "display.h"

namespace nboul {
namespace display {

Display::Display(DisplayDriver* driver)
    : m_driver{driver}
{
}

void Display::setBackgroundColor(const Color& color)
{
    m_driver->setBackgroundColor(color);
}

void Display::setGrid(const drawable::Grid& grid, const geometry::Point& position)
{
    // m_grid = new drawable::Grid(grid.width, grid.height, grid.hDivision, grid.vDivision, grid.mainColor, grid.subColor);
    // m_gridPosition = position;
}

void Display::drawGrid()
{
    // m_driver->drawDrawable(*m_grid, m_gridPosition);
}

}  // namespace display
}  // namespace nboul
