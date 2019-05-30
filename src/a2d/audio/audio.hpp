//
// Created by selya on 23.11.2018.
//

#ifndef A2D_AUDIO_HPP
#define A2D_AUDIO_HPP

#include <soloud.h>

namespace a2d {

class Audio {
    friend class PlatformToNative;
    friend class AudioSource;

public:
    static bool Initialize();
    static void Uninitialize();

private:
    static SoLoud::Soloud audio_engine;
};

} //namespace a2d

#endif //A2D_AUDIO_HPP
