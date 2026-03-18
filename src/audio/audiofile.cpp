#include "audio/audiofile.hpp"
#include <sndfile.h>

#include <stdexcept>
#include <string>

AudioFile::AudioFile(const std::string& path)
{
    load(path);
};

void AudioFile::load(const std::string& path) {
    SF_INFO info{};

    SNDFILE* file = sf_open(path.c_str(), SFM_READ, &info);

    if(!file) {
        throw std::runtime_error("Failed to open file");
    }

    info_ = info;
    sf_count_t totalSamples = info_.frames * info_.channels;

    if (totalSamples <= 0) {
        throw std::runtime_error("Invalid sample count");
    }

    samples_.resize(static_cast<size_t>(totalSamples));
    sf_count_t read = sf_read_short(file, samples_.data(), totalSamples);

    if(read != totalSamples) {
        throw std::runtime_error("Failed to read full audio data");
    }

    if (sf_close(file) != 0) {
        throw std::runtime_error("Failed to close audio file");
    }
}

 