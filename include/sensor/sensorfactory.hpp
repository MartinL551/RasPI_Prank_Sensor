#pragma once

#include "sensor.hpp"
#include "sensorconfig.hpp"
#include<memory>

class SensorFactory{
    public:
        static std::unique_ptr<Sensor> create(const SensorConfig& config);
    private:
        static std::unique_ptr<Sensor> createHcsr04(const SensorConfig& config);
};