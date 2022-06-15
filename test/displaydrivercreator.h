#ifndef DISPLAY_TEST_DISPLAYDRIVERCREATOR_H
#define DISPLAY_TEST_DISPLAYDRIVERCREATOR_H

#include "displaydriver.h"

namespace nboul {
namespace display {
namespace test {

class DisplayDriverCreator {
public:
    enum class DriverType {
        RobotLcd = 0,
    };

    static DisplayDriver* create(DriverType type);
};

}  // namespace test
}  // namespace display
}  // namespace nboul

#endif
