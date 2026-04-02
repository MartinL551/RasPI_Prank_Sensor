#include "audio/audioplayer.hpp"
#include "sensor/hcsr04.hpp"
#include "audio/audiofile.hpp"
#include "sensor/sensorconfig.hpp"
#include "sensor/sensorfactory.hpp"
#include "sensor/sensortype.hpp"
#include "setup/setupmenu.hpp"
#include <iostream>
#include <exception>

constexpr unsigned int TRIG = 5;
constexpr unsigned int ECHO = 6;

int main() {
    try{
        AudioFile file("assets/audio/beep.wav");
        AudioPlayer player;
        player.init();
        player.configureAlsa(file.sampleRate(), file.channels_());
        SetupMenu setup;

        SetupConfig config =  setup.setup();
        

    

        while(true) {
            if(config.sensor->triggered()) {
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