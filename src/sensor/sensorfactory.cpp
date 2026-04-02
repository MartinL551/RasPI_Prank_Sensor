#include "sensor/sensorfactory.hpp"
#include "sensor/hcsr04.hpp"
#include "sensor/sensor.hpp"
#include "sensor/sensorconfig.hpp"
#include "sensor/sensortype.hpp"
#include <memory>
#include <stdexcept>


std::unique_ptr<Sensor> SensorFactory::create(const SensorConfig& config) {
    switch (config.type) {
        case SensorType::HCSR04:
            return createHcsr04(config);
    }

    throw std::runtime_error("Config Incorrect: Unknown Sensor Type");
} 


std::unique_ptr<Sensor> SensorFactory::createHcsr04(const SensorConfig& config) {
    if(config.pins.size() != 2) {
        throw std::runtime_error("Config Incorrect: HCSR04 requries 2 pins");
    }

    return std::make_unique<Hcsr04>(config.pins);
}