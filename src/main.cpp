#include "audio/audioplayer.hpp"
#include "sensor/hcsr04.hpp"
#include "audio/audiofile.hpp"
#include <iostream>
#include <exception>

constexpr unsigned int TRIG = 5;
constexpr unsigned int ECHO = 6;

int main() {
    try{
        AudioFile file("assets/audio/test.wav");
        AudioPlayer player;
        player.init();
        player.configureAlsa(file.sampleRate(), file.channels_());


        Hcsr04 sensor(5, 6);

        while(true) {
            if(sensor.triggered()) {
                std::cout << "Triggered!\n";
                player.play(file);
            } else {
                std::cout << "Not Triggered!\n";
            }
        }
   
    } catch(const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() <<  "\n";
        return 1;
    }

    return 0;
}