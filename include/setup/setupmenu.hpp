#pragma once

#include "setup/setupconfig.hpp"
#include "audio/audioplayer.hpp"
#include "sensor/sensor.hpp"
#include <memory>

class SetupMenu{
    public:
        SetupConfig setup();
    private:
        AudioPlayer setupPlayer();
        std::unique_ptr<Sensor> setupSensor();
};