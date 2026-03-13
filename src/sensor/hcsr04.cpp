#include "sensor/hcsr04.hpp"

Hcsr04::Hcsr04(int trig_pin, int echo_pin)
    : trig_pin_(trig_pin),
      echo_pin_(echo_pin),
      pulse_duration_(10),
      timeout_duration_(50000),
      chip_(nullptr),
      req_conf_(nullptr),
      trig_settings_(nullptr),
      trig_line_conf_(nullptr),
      trig_req_(nullptr),
      echo_settings_(nullptr),
      echo_line_conf_(nullptr),
      echo_req_(nullptr)
      {
      }

double Hcsr04::read_distance_cm(){
    return 0.0;
}

bool Hcsr04::triggered(){
    return read_distance_cm() > 50;
}