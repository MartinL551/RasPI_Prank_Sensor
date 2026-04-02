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
};




std::unique_ptr<Sensor> SetupMenu::setupSensor() {
    SensorOption option = promptSensorType();
    std::vector<PinConfig> pinConfig = promptSensorPins(option);
    SensorConfig sensorConfig;

    sensorConfig.type = option.type;
    sensorConfig.pins = pinConfig;

    return SensorFactory::create(sensorConfig);
}

std::vector<PinConfig> SetupMenu::promptSensorPins(SensorOption option) {
    std::vector<PinOption> reqPins = sensor::getRequiredPins(option.type);
    std::vector<PinConfig> pinConfigs;

    for(size_t i = 0; i < reqPins.size();  i++) {
        const auto& pinOption = reqPins[i];
        unsigned int mappedPin = promptSensorPin(option.name, pinOption);
        const PinConfig& pinConfig = {pinOption.role, mappedPin}; 
        pinConfigs.push_back(pinConfig);
    }

    return pinConfigs;
}


unsigned int SetupMenu::promptSensorPin(std::string sensorName, PinOption option) {
    while (true) {
        std::cout << "Pick Pi Mapping for Sensor " << sensorName << " and needed sensor pin " << option.name << "\n";
        
        //40 is GPIO count of Pi pins
        unsigned int choice = SetupMenu::promptInt("> ", 40);

        return choice;
    }
}

SensorOption SetupMenu::matchSensorType(int choice) {
    for(size_t i = 0; i < SENSOR_OPTIONS.size(); i++ ){
        const auto& option = SENSOR_OPTIONS[i];
        
        if(i == static_cast<size_t>(choice - 1)) {
            return option;
        }
    }
    
    throw std::runtime_error("Invalid sensor selection");
}


SensorOption SetupMenu::promptSensorType() {
    while (true) {
        for(size_t i = 0; i < SENSOR_OPTIONS.size(); i++) {
            const auto& option = SENSOR_OPTIONS[i];
            std::cout << (i + 1) << ". " << option.name << "\n";
        }

        unsigned int choice = SetupMenu::promptInt("> ", SENSOR_OPTIONS.size());

        try {
            return SetupMenu::matchSensorType(choice);
        } catch (...) {
            std::cout << "Invalid option, try again";
        }
    }
}

int SetupMenu::promptInt(std::string msg, size_t maxValue) {
    unsigned int value;

    while (true) {
        std::cout << msg;

        if(std::cin >> value) {
            if(value < 1 || static_cast<size_t>(value) > maxValue) {
                std::cout << "Must be a valid Integer value and larger than 0";
            } else {
                 return value;
            }       
        }

        std::cout << "Must be a valid Integer value";
       
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}



