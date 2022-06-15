#ifndef NOUL_DISPLAY_ROBOTLCDDISPLAYDRIVER_H
#define NOUL_DISPLAY_ROBOTLCDDISPLAYDRIVER_H

#include "displaydriver.h"

#include "TFT.h"

namespace nboul {
namespace display {

class RobotLcdDisplayDriver : public DisplayDriver {
public:
    RobotLcdDisplayDriver(geometry::Coordinate width, geometry::Coordinate height, uint8_t blPin, uint8_t resetPin, uint8_t dcPin, uint8_t csPin);
    void drawBackground() override;

private:
    TFT* m_tft = nullptr;
    uint8_t m_blPin = 0;  // backlight pin

    void drawPoint(const geometry::Point& point, const Color& color) override;
    void drawLine(const geometry::Line &line, const Color &color) override;
    void drawRectangle(const geometry::Rectangle &rectangle, geometry::Coordinate radius,
                       const nonstd::optional<Color> &strikeColorOpt, const nonstd::optional<Color> &fillColorOpt) override;
};

}  // namespace display
}  // namespace nboul

#endif  // NOUL_DISPLAY_ROBOTLCDDISPLAYDRIVER_H
