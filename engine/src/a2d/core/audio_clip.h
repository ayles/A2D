//
// Created by selya on 30.11.2018.
//

#ifndef A2D_AUDIO_CLIP_H
#define A2D_AUDIO_CLIP_H

#include <a2d/core/macro.h>

#include <soloud_wav.h>

#include <vector>

namespace a2d {

DECLARE_SMART_POINTER(AudioClip)

class AudioClip : public ref_counter {
    friend class AudioSource;

public:
    AudioClip(const std::vector<unsigned char> &audio_file);
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(AudioClip)
    ~AudioClip() override;

private:
    SoLoud::Wav data;
};

} //namespace a2d

#endif //A2D_AUDIO_CLIP_H
