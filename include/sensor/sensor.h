#pragma once

class Sensor{
    public:
        virtual ~Sensor() = default;

        virtual bool triggered() = 0;
};