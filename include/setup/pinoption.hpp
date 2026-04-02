#pragma once

#include "sensor/pinrole.hpp"
#include <string>


struct PinOption {
    PinRole role;
    std::string name;
};