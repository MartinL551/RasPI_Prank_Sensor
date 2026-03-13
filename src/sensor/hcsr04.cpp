#include "sensor/hcsr04.hpp"
#include <gpiod.h>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <thread>

Hcsr04::Hcsr04(unsigned int trig_pin, unsigned int echo_pin)
    : trig_pin_(trig_pin),
      echo_pin_(echo_pin),
      pulse_duration_(10),
      timeout_duration_(5000),
      chip_(nullptr),
      req_conf_(nullptr),
      trig_settings_(nullptr),
      trig_line_conf_(nullptr),
      trig_req_(nullptr),
      echo_settings_(nullptr),
      echo_line_conf_(nullptr),
      echo_req_(nullptr)
      {
        try {
            initChip();
            initReqConf();
            initTrig();
            initEcho();
        } catch (...) {
            cleanup();
            throw;
        }
      }

double Hcsr04::read_distance_cm(){
    return 0.0;
}

bool Hcsr04::triggered(){
    return true;
}

void Hcsr04::initChip() {
    // gpio chip object
   chip_  = gpiod_chip_open("/dev/gpiochip0");

   if(!chip_) {
        throw std::runtime_error("Failed to create object for chip /dev/gpiochip0");
   }
}

void Hcsr04::initReqConf() {
    //setup req config
    req_conf_ = gpiod_request_config_new();

    if(!req_conf_) {
        throw std::runtime_error("Failed create request config");
    }
}

void Hcsr04::initTrig() {
    //Setup trigger
    trig_settings_ = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(trig_settings_, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(trig_settings_, GPIOD_LINE_VALUE_INACTIVE);

    trig_line_conf_ = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(trig_line_conf_, &trig_pin_, 1, trig_settings_);
    
    trig_req_ = gpiod_chip_request_lines(chip_, req_conf_, trig_line_conf_);

    if(!trig_req_) {
        throw std::runtime_error("Failed to init trigger");
    }
}

void Hcsr04::initEcho() {
    //Setup echo
    echo_settings_ = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(echo_settings_, GPIOD_LINE_DIRECTION_INPUT);

    echo_line_conf_ = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(echo_line_conf_, &echo_pin_, 1, echo_settings_);

    echo_req_ = gpiod_chip_request_lines(chip_, req_conf_, echo_line_conf_);

    if(!echo_req_) {
        throw std::runtime_error("Failed to init echo");
    }
}

void Hcsr04::cleanup() {
    if(echo_req_) {
        gpiod_line_request_release(echo_req_);
        echo_req_ = nullptr;
    }
    
    if(echo_line_conf_){
        gpiod_line_config_free(echo_line_conf_);
        echo_line_conf_ = nullptr;
    }

    if(echo_settings_){
        gpiod_line_settings_free(echo_settings_);
        echo_settings_ = nullptr;
    }

    if(trig_req_) {
        gpiod_line_request_release(trig_req_);
        trig_req_ = nullptr;
    }

    if(trig_line_conf_){
        gpiod_line_config_free(trig_line_conf_);
        trig_line_conf_ = nullptr;
    }

    if(trig_settings_){
        gpiod_line_settings_free(trig_settings_);
        trig_settings_ = nullptr;
    }

    if(req_conf_) {
        gpiod_request_config_free(req_conf_);
        req_conf_ = nullptr;
    }

    if(chip_) {
        gpiod_chip_close(chip_);
        chip_ = nullptr;
    }
}

Hcsr04::~Hcsr04() {
    cleanup();
}


