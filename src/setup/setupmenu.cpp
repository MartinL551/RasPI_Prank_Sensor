#include "setup/setupmenu.hpp"
#include "sensor/pinconfig.hpp"
#include "sensor/pinrole.hpp"
#include "sensor/sensorconfig.hpp"
#include "setup/pinoption.hpp"
#include "setup/sensoroption.hpp"
#include "setup/setupconfig.hpp"
#include "sensor/sensortype.hpp"
#include "sensor/sensorfactory.hpp"
#include "sensor/sensormetadata.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <limits>
#include <vector>


SetupConfig SetupMenu::setup() {
    SetupConfig config;

    config.sensor = setupSensor();

    return config;
}


std::unique_ptr<Sensor> SetupMenu::setupSensor() {
    SensorOption option = promptSensorType();
    std::vector<PinConfig> pinConfig = promptSensorPins(option);
    SensorConfig sensorConfig;

    sensorConfig.type = option.type;
    sensorConfig.pins = pinConfig;

    return SensorFactory::create(sensorConfig);
}

std::vector<PinConfig> SetupMenu::promptSensorPins(const SensorOption& option) {
    std::vector<PinOption> reqPins = sensor::getRequiredPins(option.type);
    std::vector<PinConfig> pinConfigs;

    for(size_t i = 0; i < reqPins.size();  i++) {
        const auto& pinOption = reqPins[i];
        unsigned int mappedPin = promptSensorPin(option.name, pinOption);
        PinConfig pinConfig = {pinOption.role, mappedPin}; 
        pinConfigs.push_back(pinConfig);
    }

    return pinConfigs;
}


unsigned int SetupMenu::promptSensorPin(std::string sensorName, const PinOption& option) {
    std::cout << "Pick Pi (GPIO number) Mapping for Sensor " << sensorName << " and needed sensor pin " << option.name << "\n";
    
    //40 is GPIO count of Pi pins
    unsigned int choice = promptInt("> ", 40);

    return choice;
}

SensorOption SetupMenu::matchSensorType(unsigned int choice) {
    size_t index = static_cast<size_t>(choice - 1);

    if (index >= SENSOR_OPTIONS.size()) {
        throw std::runtime_error("Invalid sensor selection");
    }

    return SENSOR_OPTIONS[index];
}


SensorOption SetupMenu::promptSensorType() {
    while (true) {
        std::cout << "Please pick a sensor from the below options" << "\n";

        for(size_t i = 0; i < SENSOR_OPTIONS.size(); i++) {
            const auto& option = SENSOR_OPTIONS[i];
            std::cout << (i + 1) << ". " << option.name << "\n";
        }

        unsigned int choice = promptInt("> ", SENSOR_OPTIONS.size());

        try {
            return matchSensorType(choice);
        } catch (...) {
            std::cout << "Invalid option, try again";
        }
    }
}

unsigned int SetupMenu::promptInt(const std::string msg, size_t maxValue) {
    unsigned int value;

    while (true) {
        std::cout << msg;

        if (std::cin >> value) {
            if (value >= 1 && static_cast<size_t>(value) <= maxValue) {
                return value;
            }

            std::cout << "Value must be between 1 and " << maxValue << "\n";
        } else {
            std::cout << "Must be a valid integer value\n";
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}


