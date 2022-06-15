#include "robotlcddisplaydriver.h"
#include "platform_logger.h"
#include "geometry/rectangle.h"
#include "geometry/coordinate.h"

#include <Arduino.h>

namespace nboul {
namespace display {

RobotLcdDisplayDriver::RobotLcdDisplayDriver(geometry::Coordinate width, geometry::Coordinate height, uint8_t blPin, uint8_t resetPin, uint8_t dcPin, uint8_t csPin)
    : DisplayDriver{width, height}
    , m_tft{new TFT{csPin, dcPin, resetPin}}
    , m_blPin{blPin}
{
    loginfo("RobotLcdDisplayDriver display size: %dx%d\n", width, height);

    m_tft->begin();
    m_tft->setRotation(3);
    analogWrite(blPin, 128);
}

void RobotLcdDisplayDriver::drawBackground()
{
    m_tft->background(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b);
}

void RobotLcdDisplayDriver::drawPoint(const geometry::Point& point, const Color& color)
{
    m_tft->drawPixel(static_cast<int16_t>(point.x), static_cast<int16_t>(point.y), color.toRgb565());
}

void RobotLcdDisplayDriver::drawLine(const geometry::Line& line, const Color& color)
{
    m_tft->drawLine(static_cast<int16_t>(line.p0.x), static_cast<int16_t>(line.p0.y), static_cast<int16_t>(line.p1.x), static_cast<int16_t>(line.p1.y), color.toRgb565());
}

void RobotLcdDisplayDriver::drawRectangle(const geometry::Rectangle& rectangle, geometry::Coordinate radius, const nonstd::optional<Color>& strokeColorOpt, const nonstd::optional<Color>& fillColorOpt)
{
    if (strokeColorOpt) {
        m_tft->stroke(strokeColorOpt->toRgb565());
        m_tft->rect(rectangle.p0.x, rectangle.p0.y, rectangle.p1.x - rectangle.p0.x, rectangle.p1.y - rectangle.p0.y, radius);
    }

    if (fillColorOpt)
        m_tft->fillRect(rectangle.p0.x, rectangle.p0.y, rectangle.p1.x - rectangle.p0.x, rectangle.p1.y - rectangle.p0.y, fillColorOpt->toRgb565());
}

/*
void RobotLcdDisplayDriver::drawLine(const drawableprimitive::Line& line, const geometry::Point& position)
{
    m_tft->line(static_cast<int16_t>(line.line.p0.x + position.x),
                static_cast<int16_t>(line.line.p0.y + position.y),
                static_cast<int16_t>(line.line.p1.x + position.x),
                static_cast<int16_t>(line.line.p1.y + position.y));
}

void RobotLcdDisplayDriver::drawColorStrike(const drawableprimitive::ColorStrike& colorStrike)
{
    m_colorStrikeOpt = m_eraseMode ? m_backgroundColor : colorStrike.colorOpt;
    if (m_colorStrikeOpt)
        m_tft->stroke(m_colorStrikeOpt->r, m_colorStrikeOpt->g, m_colorStrikeOpt->b);
}

void RobotLcdDisplayDriver::drawColorFill(const drawableprimitive::ColorFill& colorFill)
{
    m_colorFillOpt = m_eraseMode ? m_backgroundColor : colorFill.colorOpt;
    if (m_colorFillOpt)
        m_tft->fill(m_colorFillOpt->r, m_colorFillOpt->g, m_colorFillOpt->b);
}
*/

}  // namespace display
}  // namespace nboul
