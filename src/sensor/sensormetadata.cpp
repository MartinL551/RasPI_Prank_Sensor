
#include "sensor/pinrole.hpp"
#include "sensor/sensortype.hpp"
#include "setup/pinoption.hpp"
#include <stdexcept>
#include <vector>

namespace sensor {
    std::vector<PinOption> getRequiredPins(SensorType type) {
        switch (type) {
            case SensorType::HCSR04:
                PinOption Trig = {
                    PinRole::TRIG,
                    "TRIG"
                };

                PinOption Echo = {
                    PinRole::ECHO,
                    "ECHO"
                };
                return {Trig, Echo};
        }

        throw std::runtime_error("Unknown sensor type");
    }
}
