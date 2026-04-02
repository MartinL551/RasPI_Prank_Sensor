#pragma once

#include "sensortype.hpp"
#include "setup/pinoption.hpp"
#include <vector>

namespace sensor {
    std::vector<PinOption> getRequiredPins(SensorType type);
}
