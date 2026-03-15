#pragma once

#include <string>
#include <vector>

class AudioPlayer {
    AudioPlayer(std::string& path);
    public:
        bool init();
        bool load();
        bool play();

    private:
        bool configureAsla();
        
        std::string path_;
        std::vector<short> buffer_;
        int sampleRate_ = 0;
        int channels_ = 0;
};