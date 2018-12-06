//
// Created by selya on 30.11.2018.
//

#ifndef A2D_AUDIO_SOURCE_H
#define A2D_AUDIO_SOURCE_H

#include <a2d/core/component.h>
#include <a2d/core/audio_clip.h>
#include <a2d/core/audio.h>

#include <soloud.h>

namespace a2d {

class AudioSource : public Component {
    pAudioClip audio_clip;
    float volume;
    float pan;

    SoLoud::handle handle;
    bool paused;

public:
    AudioSource();

    void SetAudioClip(const pAudioClip &audio_clip);
    void SetVolume(float volume);
    void SetPan(float pan);

    const pAudioClip &GetAudioClip();
    float GetVolume();
    float GetPan();

    void Play();
    void Pause();
    void Stop();
};

} //namespace a2d

#endif //A2D_AUDIO_SOURCE_H
