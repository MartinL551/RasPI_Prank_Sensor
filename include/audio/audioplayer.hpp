#pragma once

#include <alsa/asoundlib.h>
#include "audio/audiofile.hpp"
class AudioPlayer {
    public:
        // ~AudioPlayer();
        
        void init();
        void play(const AudioFile& audio);
        void configureAlsa(int sampleRate, int channels);
        void close();
    private:
        snd_pcm_t* handle_ = nullptr;
};  