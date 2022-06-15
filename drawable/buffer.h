#ifndef NBOUL_DISPLAY_DRAWABLEPRIMITIVE_BUFFER_H_
#define NBOUL_DISPLAY_DRAWABLEPRIMITIVE_BUFFER_H_

#include "drawable/drawable.h"
#include "geometry/coordinate.h"
#include "sample.h"
#include "color/color.h"

#include <vector>

namespace nboul {
namespace display {
namespace drawable {

class Buffer : public Drawable {

public:
    Buffer(const geometry::Point& position, sample::Sample minSampleValue, sample::Sample maxSampleValue, geometry::Coordinate minY, geometry::Coordinate maxY, const Color& color)
        : Drawable{position}
        , minSampleValue{minSampleValue}
        , maxSampleValue{maxSampleValue}
        , minY{minY}
        , maxY{maxY}
        , color{color}
    {
    }

    void clearSamples()
    {
        m_samples.clear();
    }

    void draw(const geometry::Rectangle &rect, DisplayDriver &driver)
    {
        geometry::Coordinate y = map(m_samples[m_drawXStart]);
        geometry::Coordinate prevY = y;

        for (geometry::Coordinate x = m_drawXStart + 1; x <= m_drawXEnd; x++) {
            y = map(m_samples[x]);
            const geometry::Line& line{{position.x + x - 1, position.y + prevY}, {position.x + x, position.y + y}};
            driver.drawLine(line, color);
            prevY = y;
        }
    }

    void move(const geometry::Point &position) override
    {
    }

    void setSampleAt(geometry::Coordinate x, sample::Sample sample)
    {
        if (static_cast<sample::Sample>(m_samples.size()) > x) {
            // Sample index already exists, overwrite it
            m_samples[x] = sample;
        } else {
            // Fill with empty samples until requested position
            while (static_cast<sample::Sample>(m_samples.size()) < x)
                m_samples.push_back(minSampleValue);
            m_samples.push_back(sample);
        }
    }

    void setDrawRange(geometry::Coordinate xStart, geometry::Coordinate xEnd)
    {
        m_drawXStart = xStart;
        m_drawXEnd = xEnd;
    }

    sample::Sample minSampleValue;
    sample::Sample maxSampleValue;
    geometry::Coordinate minY;
    geometry::Coordinate maxY;
    Color color;

private:
    geometry::Coordinate map(sample::Sample sample) const
    {
        if (sample >= maxSampleValue)
            return maxY;
        if (sample <= minSampleValue)
            return minY;
        return (maxY - minY) * (sample - minSampleValue) / (maxSampleValue - minSampleValue) + minY;
    }

    int m_drawXStart = 0;
    int m_drawXEnd = 0;
    std::vector<sample::Sample> m_samples;
};

}  // namespace drawableprimitive
}  // namespace display
}  // nboul

#endif  // NBOUL_DISPLAY_DRAWABLEPRIMITIVE_BUFFER_H_
