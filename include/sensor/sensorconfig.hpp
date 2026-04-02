#pragma once

#include "sensor/pinconfig.hpp"
#include "sensortype.hpp"
#include <vector>


struct SensorConfig {
    SensorType type;
    std::vector<PinConfig> pins;
};