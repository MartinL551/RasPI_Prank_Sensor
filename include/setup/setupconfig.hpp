#pragma once

#include "sensor/sensor.hpp"
#include "audio/audioplayer.hpp"
#include<memory>


struct SetupConfig {
    std::unique_ptr<Sensor> sensor;
    AudioPlayer player;
};