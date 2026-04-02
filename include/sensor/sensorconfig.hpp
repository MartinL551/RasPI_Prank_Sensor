#pragma once

#include "sensortype.hpp"
#include <vector>


struct SensorConfig {
    SensorType type;
    std::vector<unsigned int> pins;
};