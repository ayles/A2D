//
// Created by selya on 05.12.2018.
//

#include <a2d/core/audio.hpp>

namespace a2d {

SoLoud::Soloud Audio::audio_engine;

bool Audio::Initialize() {
    audio_engine.init();
    return true;
}

void Audio::Uninitialize() {
    audio_engine.deinit();
}

} //namespace a2d
