//
// Created by selya on 30.11.2018.
//

#ifndef A2D_AUDIO_CLIP_H
#define A2D_AUDIO_CLIP_H

#include <a2d/core/macro.h>
#include <a2d/core/component.h>

namespace a2d {

DECLARE_SMART_POINTER(AudioClip)

class AudioClip {
public:


    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(AudioClip)

    AudioClip();

    ~AudioClip();
};

} //namespace a2d

#endif //A2D_AUDIO_CLIP_H
