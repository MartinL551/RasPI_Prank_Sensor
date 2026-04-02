#pragma once

#include "sensor/sensortype.hpp"
#include <string>
#include <vector>

struct SensorOption {
    SensorType type;
    std::string name;
};

inline const std::vector<SensorOption> SENSOR_OPTIONS = {
    {SensorType::HCSR04, "HC-SR04"}
};