//
// Created by selya on 30.11.2018.
//

#ifndef A2D_AUDIO_CLIP_HPP
#define A2D_AUDIO_CLIP_HPP

#include <a2d/core/macro.hpp>

#include <soloud_wav.h>

#include <vector>

namespace a2d {

DECLARE_SMART_POINTER(AudioClip)

class AudioClip : public ref_counter {
    friend class AudioSource;
    friend class Resources;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(AudioClip)

private:
    AudioClip(const std::vector<unsigned char> &audio_file);
    ~AudioClip() override;
    SoLoud::Wav data;
};

} //namespace a2d

#endif //A2D_AUDIO_CLIP_HPP
