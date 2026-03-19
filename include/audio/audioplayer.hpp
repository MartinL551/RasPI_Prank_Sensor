#pragma once

#include <alsa/asoundlib.h>
#include "audio/audiofile.hpp"
class AudioPlayer {
    public:
        AudioPlayer();
        ~AudioPlayer();
        
        bool init();
        bool play(const AudioFile& audio);
    private:
        bool configureAlsa(int sampleRate, int channels);

        snd_pcm_t* handle_ = nullptr;
};  