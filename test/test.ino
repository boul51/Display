#include <Arduino.h>
#include "platform_logger.h"

#include <MemoryFree.h>

#include "display.h"
#include "color/color.h"
#include "drawable/grid.h"
#include "drawable/buffer.h"
#include "drawable/rectangle.h"
#include "drawable/point.h"
#include "drawable/line.h"
#include "robotlcddisplaydriver.h"
#include "displaydrivercreator.h"
#include "sample.h"
#include "canvas/canvas.h"

#if HAS_LOGGER != 1
#define logdebug(...)
#define loginfo(...)
#define logwarning(...)
#define logerror(...)
#endif

#include <vector>
#include <chrono>
#include <limits>

/**** DEFINES ****/

#ifndef SERIAL_IFACE
#error "Please define SERIAL_IFACE to Serial (programming port) or SerialUSB (native port)"
#endif

using namespace nboul::display;
using namespace nboul::sample;

namespace {
const test::DisplayDriverCreator::DriverType DriverType = test::DisplayDriverCreator::DriverType::RobotLcd;
}  // namespace

namespace std {

void __throw_bad_alloc()
{
    logerror("__throw_bad_alloc\n");
    while (true) {}
}

void __throw_length_error(char const*)
{
    logerror("__throw_length_error\n");
    while (true) {}
}
}  // namespace std

Display* g_display;

void testBgColor(const std::chrono::milliseconds& duration)
{
    loginfo("%s++\n", __FUNCTION__);

    const std::vector<Color> colors = {
        colors::Red,
        colors::Green,
        colors::Blue,
    };

    for (const auto& color: colors) {
        g_display->setBackgroundColor(color);
        delay(duration.count() / colors.size());
    }

    loginfo("%s--\n", __FUNCTION__);
}

void testPoint(const std::chrono::milliseconds& duration)
{
    loginfo("%s++\n", __FUNCTION__);

    geometry::Coordinate offset = 10;
    geometry::Coordinate x0 = offset;
    geometry::Coordinate x1 = g_display->driverConst()->width() - offset;
    geometry::Coordinate y0 = offset;
    geometry::Coordinate y1 = g_display->driverConst()->height() - offset;

    // drawableprimitive::Point p1{{x0, y0}, colors::Green};
    // drawableprimitive::Point p2{{x1, y1}, colors::Red};

    g_display->setBackgroundColor(colors::White);
    const int loops = 100;
    for (int i = 0; i < loops; i++) {
        g_display->driver()->drawPoint({x0 + i, y0 + i}, colors::Green);
        g_display->driver()->drawPoint({x1 - i, y1 - i}, colors::Red);
        delay(duration.count() / loops);
    }

    loginfo("%s--\n", __FUNCTION__);
}

void testLine(const std::chrono::milliseconds& duration)
{
    loginfo("%s++\n", __FUNCTION__);

    geometry::Coordinate offset = 20;
    geometry::Coordinate x0 = offset;
    geometry::Coordinate x1 = g_display->driverConst()->width() - offset;
    geometry::Coordinate y0 = offset;
    geometry::Coordinate y1 = g_display->driverConst()->height() - offset;

    g_display->setBackgroundColor(colors::Black);
    g_display->driver()->drawLine({{x0, y0}, {x1, y1}}, colors::Cyan);
    g_display->driver()->drawLine({{x0, y0 + 10}, {x1, y1 + 10}}, colors::Cyan);

    delay(duration.count());

    loginfo("%s--\n", __FUNCTION__);
}

void testBufferAllSamples(const std::chrono::milliseconds& duration)
{
    loginfo("%s++\n", __FUNCTION__);

    Sample minSampleValue = 0;
    Sample maxSampleValue = std::numeric_limits<Sample>::max();
    geometry::Coordinate minY = 0;
    geometry::Coordinate maxY = g_display->driver()->height() - 1;
    int samplesCount = g_display->driver()->width();
    drawable::Buffer buffer{{0, 0}, minSampleValue, maxSampleValue, minY, maxY, colors::Magenta};
    buffer.setDrawRange(0, samplesCount - 1);

    g_display->driver()->setBackgroundColor(colors::White);

    int maxLoops = 0;  // 0 => draw until duration is elapsed, >0 => draw maxLoops loops
    int loops = 0;
    float pi = acos(-1.0);
    const int periodsPerScreen = 2;
    float phasePerLoop = - pi / 15;
    const float saturation = 1.3;

    int tStart = millis();

    while (millis() - tStart < duration.count() && (loops < maxLoops || maxLoops == 0)) {
        float phase = loops * phasePerLoop;
        buffer.clearSamples();
        for (int i = 0; i < samplesCount; i++) {
            float teta = 2 * pi * periodsPerScreen / samplesCount * i;
            DoubleSample dSample = (buffer.maxSampleValue * (0.5 + sin(teta + phase) * 0.5 * saturation));
            buffer.setSampleAt(i, clipped(dSample));
        }
        buffer.draw({{0, 0}, {0, 0}}, *g_display->driver());
        //buffer.updatePrimitives();
        //g_display->driver()->drawDrawable(buffer);
        loops++;
    }

    int elapsed = millis() - tStart;

    loginfo("%s--, drawn %d loops in %d ms (%d ms/loop)\n", __FUNCTION__, loops, elapsed, elapsed / loops);
}

void testBufferSingleSample(const std::chrono::milliseconds& duration)
{
    loginfo("%s++\n", __FUNCTION__);

    Sample minSampleValue = 0;
    Sample maxSampleValue = std::numeric_limits<Sample>::max();
    geometry::Coordinate minY = 0;
    geometry::Coordinate maxY = g_display->driver()->height() - 1;
    int samplesCount = g_display->driver()->width();
    drawable::Buffer buffer{{0, 0}, minSampleValue, maxSampleValue, minY, maxY, colors::Magenta};

    g_display->driver()->setBackgroundColor(colors::White);

    int maxLoops = 0;  // 0 => draw until duration is elapsed, >0 => draw maxLoops loops
    float pi = acos(-1.0);
    const int periodsPerScreen = 2;
    const float saturation = 0.95;

    Sample* data = new Sample[samplesCount];
    for (int i = 0; i < samplesCount; i++) {
        float teta = 2 * pi * periodsPerScreen / samplesCount * i;
        DoubleSample dSample = (buffer.maxSampleValue * (0.5 + sin(teta) * 0.5 * saturation));
        data[i] = clipped(dSample);
    }

    int tStart = millis();
    int loops = 0;

    drawable::Grid grid{{0, 0}, g_display->driver()->width(), g_display->driver()->height(), {2, 4}, {2, 4}, colors::Red, colors::Green};

    while (millis() - tStart < duration.count() && (loops < maxLoops || maxLoops == 0)) {
        g_display->driver()->drawBackground();
        grid.draw({{0, 0}, {0, 0}}, *g_display->driver());
        buffer.setSampleAt(0, data[0]);
        for (geometry::Coordinate x = 1; x < samplesCount; x++) {
            buffer.setSampleAt(x, data[x]);
            buffer.setDrawRange(x - 1, x);
            //buffer.updatePrimitives();
            buffer.draw({{0, 0}, {0, 0}}, *g_display->driver());
			//g_display->driver()->drawDrawable(buffer);
            // canvas.draw();
            delay(10);
        }
        delay(300);
        // buffer.setDrawRange(0, samplesCount - 1);
        // buffer.updatePrimitives();
        // g_display->driver()->eraseDrawable(buffer);

        loops++;
    }

    delete data;

    int elapsed = millis() - tStart;

    loginfo("%s--, drawn %d loops in %d ms (%d ms/loop)\n", __FUNCTION__, loops, elapsed, elapsed / loops);
}

void testGrid(const std::chrono::milliseconds& duration)
{
    loginfo("%s++\n", __FUNCTION__);

    g_display->setBackgroundColor(colors::Black);

    geometry::Coordinate gridWidth = g_display->driver()->width() * 4 / 5;
    geometry::Coordinate gridHeight = g_display->driver()->height() * 4 / 5;
    geometry::Point gridPosition{(g_display->driver()->width() - gridWidth) / 2,
                                 (g_display->driver()->height() - gridHeight) / 2};

    drawable::Grid grid {
        gridPosition,
        gridWidth,
        gridHeight,
        drawable::Grid::Division{3, 6},
        drawable::Grid::Division{2, 4},
        colors::White,
        colors::White.darker(1.3),
    };

    grid.draw({{0, 0}, {0, 0}}, *g_display->driver());

    delay(duration.count());

    loginfo("%s--\n", __FUNCTION__);
}

void testRectangle(const std::chrono::milliseconds& duration)
{
    loginfo("%s++\n", __FUNCTION__);

    g_display->setBackgroundColor(colors::Magenta);

    /*
    drawable::Rectangle rectangle1{{10, 10}, 100, 50, 4, colors::None, colors::Blue};
    rectangle1.draw({0, 0, 0, 0}, *g_display->driver());

    drawable::Rectangle rectangle2{{10, 70}, 60, 20, 3, colors::Yellow, colors::None};
    rectangle2.draw({0, 0, 0, 0}, *g_display->driver());
    */

    drawable::Rectangle rectangle3{{10, 5}, {11, 6}, 0, colors::None, colors::Yellow};
    //rectangle3.draw({10, 10, 1, 1}, *g_display->driver());

    delay(duration.count());

    loginfo("%s--\n", __FUNCTION__);
}

#if 0

void testRemoveDrawable(const std::chrono::milliseconds& duration)
{
    loginfo("%s++\n", __FUNCTION__);

    int steps = 3;
    const auto& stepDuration = std::chrono::milliseconds{duration.count() / steps};

    g_display->setBackgroundColor(colors::Yellow);

    Canvas canvas(*g_display->driver());

    drawable::Grid grid{g_display->driver()->width() - 1, g_display->driver()->height() - 1, {3, 6}, {3, 6}, colors::Black, colors::Cyan};
    drawable::Rectangle r1{100, 50, 0, colors::None, colors::Blue};
    drawable::Rectangle r2{100, 50, 0, colors::Red, colors::Green};

    canvas.addDrawable(&r1, {10, 10}, 2);
    canvas.addDrawable(&r2, {20, 20}, 3);
    canvas.addDrawable(&grid, {0, 0}, 1);

    canvas.draw();

    delay(stepDuration.count());

    canvas.removeDrawable(&r1);

    delay(stepDuration.count());

	canvas.removeDrawable(&r2);

    delay(stepDuration.count());

    loginfo("%s--\n", __FUNCTION__);
}

void testMoveDrawable(const std::chrono::milliseconds& duration)
{
	loginfo("%s++\n", __FUNCTION__);

	g_display->setBackgroundColor(colors::Magenta);

	Canvas canvas(*g_display->driver());

	drawable::Grid grid{g_display->driver()->width() - 1, g_display->driver()->height() - 1, {3, 6}, {3, 6}, colors::Black, colors::Cyan};

	geometry::Coordinate x = 0;
	geometry::Coordinate y = 0;
	geometry::ZCoordinate z = 2;

	drawable::Rectangle r1{20, 20, 0, colors::None, colors::Cyan};

	canvas.addDrawable(&r1, {x, y}, z);
	canvas.addDrawable(&grid, {0, 0}, 1);

	const auto& t0 = millis();

	while (millis() - t0 < duration.count() * 3) {
		x++;
		y++;
		canvas.moveDrawable(&r1, {x, y}, z);
		delay(300);
	}

	loginfo("%s--\n", __FUNCTION__);
}
#endif

void testCanvas(std::chrono::milliseconds duration)
{
    loginfo("%s++\n", __FUNCTION__);

    canvas::Canvas canvas(*g_display->driver());

    const geometry::Point displayCorner{g_display->driver()->width(), g_display->driver()->height()};

    drawable::Rectangle background{{0, 0}, displayCorner, 0, colors::None, colors::Green};

    canvas.addDrawable(&background, 0);

    canvas.draw();

    // screen size: 160 x 128
    std::vector<drawable::Point> points;
    for (geometry::Coordinate x = 0; x < 2; x++) {
        for (geometry::Coordinate y = 0; y < 2; y++) {
            points.push_back({{x, y}, colors::Red});
        }
    }

    for (auto& point : points)
        canvas.addDrawable(&point, 1);

    drawable::Rectangle rect{{0, 0}, {100, 100}, 0, colors::None, colors::Yellow};
    // canvas.addDrawable(&rect, 2);

    // drawable::Line line{{0, 0}, {g_display->driver()->width() / 3, g_display->driver()->height() / 2}, colors::Blue};
    drawable::Line line{{0, 0}, {0, g_display->driver()->height() / 2}, colors::Blue};
    canvas.addDrawable(&line, 3);

    canvas.draw();

    const auto& t0 = millis();

    const geometry::Point pointsIncrement{2, 1};
    const geometry::Point lineIncrement{1, 2};

    while (millis() - t0 < duration.count() * 3) {
        for (auto& point : points)
            point.move((point.position + pointsIncrement) % displayCorner);
        line.move((line.position + lineIncrement) % displayCorner);
        canvas.draw();
        delay(15);
    }

    /*
    const auto& t1 = millis();

    while (millis() - t1 < duration.count()) {
        line.move((line.position + pointsIncrement) % displayCorner);
        canvas.draw();
        delay(10);
    }*/
}

void setup()
{
    PlatformLogger::inst().echo(true);
    PlatformLogger::inst().auto_flush(false);
    loglevel(log_level_e::info);

    SERIAL_IFACE.begin(115200);
    while (!SERIAL_IFACE) {}
    printf_init(SERIAL_IFACE);

    loginfo("Entering setup, available memory %d\n", freeMemory());

    auto displayDriver = test::DisplayDriverCreator::create(DriverType);
    g_display = new Display(displayDriver);

    analogWriteResolution(12);

    std::chrono::milliseconds duration = std::chrono::seconds{2};

    // testRectangle(duration);
    testCanvas(duration);

#ifdef TEST_ALL
    testBgColor(duration);
    testPoint(duration);
    testLine(duration);
    testGrid(duration);
    testRectangle(duration);
    testBufferAllSamples(duration);
    testBufferSingleSample(duration * 3);
#endif  // TEST_ALL


    //testRemoveDrawable(duration);
    //testMoveDrawable(duration);

    loginfo("All tests run\n");
}

void loop()
{
}
