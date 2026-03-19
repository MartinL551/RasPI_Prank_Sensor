#pragma once

#include <sndfile.h>
#include <string>
#include <vector>

class AudioFile {
    public:
        explicit AudioFile(const std::string& path);
        const std::vector<short>& samples() const { return  samples_;};
        int sampleRate() const { return info_.samplerate;};
        int channels_() const {return info_.channels;};
        int frames_() const { return info_.frames;};

    private:
        void load(const std::string& path);
        std::vector<short> samples_;
        SF_INFO info_{};
};