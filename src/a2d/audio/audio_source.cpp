//
// Created by selya on 05.12.2018.
//

#include <a2d/audio/audio_source.hpp>
#include <a2d/core/engine.hpp>

namespace a2d {


AudioSource::AudioSource() : audio_clip(nullptr), volume(1), pan(0), handle(0), paused(true) {}

void AudioSource::SetAudioClip(const pAudioClip &audio_clip) {
    ASSERT_MAIN_THREAD
    this->audio_clip = audio_clip;
}

void AudioSource::SetVolume(float volume) {
    ASSERT_MAIN_THREAD
    this->volume = volume;
    Audio::audio_engine.setVolume(handle, volume);
}

void AudioSource::SetPan(float pan) {
    ASSERT_MAIN_THREAD
    this->pan = pan;
    Audio::audio_engine.setPan(handle, pan);
}

const pAudioClip &AudioSource::GetAudioClip() {
    ASSERT_MAIN_THREAD
    return audio_clip;
}

float AudioSource::GetVolume() {
    ASSERT_MAIN_THREAD
    return volume;
}

float AudioSource::GetPan() {
    ASSERT_MAIN_THREAD
    return pan;
}

void AudioSource::Play() {
    ASSERT_MAIN_THREAD
    if (!paused) return;
    paused = false;
    if (handle) {
        Audio::audio_engine.setPause(handle, false);
    } else {
        handle = Audio::audio_engine.play(audio_clip->data, volume, pan);
    }
}

void AudioSource::Pause() {
    ASSERT_MAIN_THREAD
    if (paused) return;
    paused = true;
    Audio::audio_engine.setPause(handle, true);
}

void AudioSource::Stop() {
    ASSERT_MAIN_THREAD
    Audio::audio_engine.stop(handle);
    paused = true;
    handle = 0;
}


} //namespace a2d
