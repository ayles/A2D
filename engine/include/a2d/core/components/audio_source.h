//
// Created by selya on 30.11.2018.
//

#ifndef A2D_AUDIO_SOURCE_H
#define A2D_AUDIO_SOURCE_H

#include <a2d/core/component.h>
#include <a2d/core/audio_clip.h>

#include <soloud.h>

namespace a2d {

class AudioSource : public Component {
    pAudioClip audio_clip;
    float volume = 1;
    float pan = 0;

    SoLoud::handle handle = 0;
    bool paused = true;

public:
    void SetAudioClip(const pAudioClip &audio_clip) {
        this->audio_clip = audio_clip;
        if (!audio_clip) return;
    }

    void SetVolume(float volume) {
        this->volume = volume;
        GetAudioEngine().setVolume(handle, volume);
    }

    void SetPan(float pan) {
        this->pan = pan;
        GetAudioEngine().setPan(handle, pan);
    }

    const pAudioClip &GetAudioClip() {
        return audio_clip;
    }

    float GetVolume() {
        return volume;
    }

    float GetPan() {
        return pan;
    }

    void Play() {
        if (!paused) return;
        paused = false;
        if (handle) {
            GetAudioEngine().setPause(handle, false);
        } else {
            handle = GetAudioEngine().play(audio_clip->data, volume, pan);
        }
    }

    void Pause() {
        if (paused) return;
        paused = true;
        GetAudioEngine().setPause(handle, true);
    }

    void Stop() {
        GetAudioEngine().stop(handle);
        paused = true;
        handle = 0;
    }

private:
    static SoLoud::Soloud &GetAudioEngine() {
        static SoLoud::Soloud engine;
        static bool initialized = false;
        if (!initialized) {
            engine.init();
            initialized = true;
        }
        return engine;
    }
};

} //namespace a2d

#endif //A2D_AUDIO_SOURCE_H
