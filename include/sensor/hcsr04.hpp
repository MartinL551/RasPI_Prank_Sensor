#pragma once

#include "sensor.hpp"
#include <gpiod.h>

class Hcsr04 : public Sensor {
    public:
        Hcsr04(int trig_pin, int echo_pin);
        ~Hcsr04() override;

        bool triggered() override;
        double read_distance_cm();

    private:
        void initChip();
        void initReqConf();
        void initTrig();
        void initEcho();
        void cleanup();

        int trig_pin_;
        int echo_pin_;
        const int pulse_duration_;
        const int timeout_duration_;

        gpiod_chip* chip_;
        gpiod_request_config* req_conf_;
        
        gpiod_line_settings* trig_settings_;
        gpiod_line_config* trig_line_conf_;
        gpiod_line_request* trig_req_;

        gpiod_line_settings* echo_settings_;
        gpiod_line_config* echo_line_conf_;
        gpiod_line_request* echo_req_;
};