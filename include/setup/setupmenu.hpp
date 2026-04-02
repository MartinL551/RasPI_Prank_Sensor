#pragma once

#include "sensor/pinconfig.hpp"
#include "sensor/sensortype.hpp"
#include "setup/pinoption.hpp"
#include "setup/setupconfig.hpp"
#include "audio/audioplayer.hpp"
#include "sensor/sensor.hpp"
#include "setup/sensoroption.hpp"
#include <cstddef>
#include <memory>

class SetupMenu{
    public:
        SetupConfig setup();
    private:
        AudioPlayer setupPlayer();
        std::unique_ptr<Sensor> setupSensor();
        SensorOption matchSensorType(unsigned int choice);
        SensorOption promptSensorType();
        unsigned int promptInt(std::string msg, size_t maxValue);
        std::vector<PinConfig> promptSensorPins(const SensorOption& option);
        unsigned int promptSensorPin(std::string sensorName, const PinOption& option);
};