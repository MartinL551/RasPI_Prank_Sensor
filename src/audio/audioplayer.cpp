#include "audio/audioplayer.hpp"
#include <alsa/asoundlib.h>
#include <sndfile.h>
#include <iostream>


void AudioPlayer::init() {
    snd_pcm_open(&handle_, "default", SND_PCM_STREAM_PLAYBACK, 0);
}

void AudioPlayer::configureAlsa(int sampleRate, int channels) {
    snd_pcm_set_params(handle_, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, channels, sampleRate, 1, 500000);
}

void AudioPlayer::play(const AudioFile& audio){
    snd_pcm_writei(handle_, audio.samples().data(), audio.frames_());
    std::cout << "Playing File for: " <<  static_cast<double>(audio.frames_()) / audio.sampleRate() << " \n";
    snd_pcm_drain(handle_);
    snd_pcm_prepare(handle_);
}

void AudioPlayer::close(){
    if(handle_) {
        snd_pcm_close(handle_);
    }
}







 