#include "displaydriver.h"
#include "platform_logger.h"
#include "sample.h"

namespace nboul {
namespace display {

DisplayDriver::DisplayDriver(geometry::Coordinate width, geometry::Coordinate height)
    : m_width{width}
    , m_height{height}
{
}

void DisplayDriver::setBackgroundColor(const Color& color)
{
    m_backgroundColor = color;
    drawBackground();
}

/*
void DisplayDriver::drawPrimitive(const drawableprimitive::Primitive& primitive)
{
    switch (primitive.type) {
    case drawableprimitive::Primitive::Type::Line:
        drawLine(reinterpret_cast<const drawableprimitive::Line&>(primitive), position);
        break;
    case drawableprimitive::Primitive::Type::Point:
        drawPoint(reinterpret_cast<const drawableprimitive::Point&>(primitive), position);
        break;
    case drawableprimitive::Primitive::Type::ColorStrike:
        drawColorStrike(reinterpret_cast<const drawableprimitive::ColorStrike&>(primitive));
        break;
    case drawableprimitive::Primitive::Type::ColorFill:
        drawColorFill(reinterpret_cast<const drawableprimitive::ColorFill&>(primitive));
        break;
    case drawableprimitive::Primitive::Type::Rectangle:
        drawRectangle(reinterpret_cast<const drawableprimitive::Rectangle&>(primitive), position);
        break;
    default:
        logerror("Don't know how to draw primitive of type %d\n", primitive.type);
        break;
    }
}

void DisplayDriver::drawDrawable(const drawable::Drawable& drawable, const geometry::Point& position)
{
    for (const drawableprimitive::Primitive* primitive: drawable.primitives)
        drawPrimitive(*primitive, position);
}

void DisplayDriver::eraseDrawable(const drawable::Drawable& drawable, const geometry::Point& position)
{
    m_eraseMode = true;
    drawDrawable(drawable, position);
    m_eraseMode = false;
}
*/

}  // namespace display
}  // namespace nboul
