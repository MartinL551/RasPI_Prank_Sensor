#include "sensor/hcsr04.hpp"
#include <iostream>
#include <exception>

constexpr unsigned int TRIG = 5;
constexpr unsigned int ECHO = 6;

int main() {
    try{
        Hcsr04 sensor(5, 6);

        if(sensor.triggered()){
            std::cout << "Triggered!";
        }
    } catch(const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() <<  "\n";
        return 1;
    }

    return 0;
}