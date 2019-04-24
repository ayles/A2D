//
// Created by selya on 05.12.2018.
//

#include <a2d/audio/audio_clip.hpp>

namespace a2d {

AudioClip::AudioClip(const std::vector<unsigned char> &audio_file) {
    if (audio_file.empty()) return;
    data.loadMem(const_cast<unsigned char *>(&audio_file[0]), audio_file.size(), true);
}

AudioClip::~AudioClip() = default;

} //namespace a2d
