//
// Created by selya on 05.12.2018.
//

#include <a2d/core/components/audio_source.h>

namespace a2d {


AudioSource::AudioSource() : audio_clip(nullptr), volume(1), pan(0), handle(0), paused(true) {}

void AudioSource::SetAudioClip(const pAudioClip &audio_clip) {
    this->audio_clip = audio_clip;
}

void AudioSource::SetVolume(float volume) {
    this->volume = volume;
    Audio::audio_engine.setVolume(handle, volume);
}

void AudioSource::SetPan(float pan) {
    this->pan = pan;
    Audio::audio_engine.setPan(handle, pan);
}

const pAudioClip &AudioSource::GetAudioClip() {
    return audio_clip;
}

float AudioSource::GetVolume() {
    return volume;
}

float AudioSource::GetPan() {
    return pan;
}

void AudioSource::Play() {
    if (!paused) return;
    paused = false;
    if (handle) {
        Audio::audio_engine.setPause(handle, false);
    } else {
        handle = Audio::audio_engine.play(audio_clip->data, volume, pan);
    }
}

void AudioSource::Pause() {
    if (paused) return;
    paused = true;
    Audio::audio_engine.setPause(handle, true);
}

void AudioSource::Stop() {
    Audio::audio_engine.stop(handle);
    paused = true;
    handle = 0;
}


} //namespace a2d
