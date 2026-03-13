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
      pulse_duration_(100),
      timeout_duration_(1000),
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

void Hcsr04::sleepAndContinue(int duration) {
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

double Hcsr04::read_distance_cm(){
    gpiod_line_request_set_value(trig_req_, trig_pin_, GPIOD_LINE_VALUE_INACTIVE);
    std::this_thread::sleep_for(std::chrono::microseconds(2));

    // 10us trigger pulse
    gpiod_line_request_set_value(trig_req_, trig_pin_, GPIOD_LINE_VALUE_ACTIVE);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    gpiod_line_request_set_value(trig_req_, trig_pin_, GPIOD_LINE_VALUE_INACTIVE);

    // Wait for ECHO high with timeout
    auto wait_start = std::chrono::steady_clock::now();
    while (gpiod_line_request_get_value(echo_req_, echo_pin_) == GPIOD_LINE_VALUE_INACTIVE) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - wait_start).count() > pulse_duration_) {
            std::cout << "Timeout waiting for echo start\n";
            sleepAndContinue(timeout_duration_);
        }
    }

    auto pulse_start = std::chrono::steady_clock::now();

    // Wait for ECHO low with timeout
    while (gpiod_line_request_get_value(echo_req_, echo_pin_) == GPIOD_LINE_VALUE_ACTIVE) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - pulse_start).count() > pulse_duration_) {
            std::cout << "Timeout waiting for echo end\n";
            sleepAndContinue(timeout_duration_);
        }
    }

    auto pulse_end = std::chrono::steady_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(pulse_end - pulse_start).count();


    return duration_us * 0.0343 / 2.0;
}

bool Hcsr04::triggered(){
    return read_distance_cm() < 50;
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


