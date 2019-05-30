//
// Created by selya on 29.05.2019.
//

#ifndef A2D_NATIVE_TO_PLATFORM_H
#define A2D_NATIVE_TO_PLATFORM_H

#include <a2d/renderer/renderer.hpp>

namespace a2d {

class NativeToPlatform {
public:
    static void SetOrientation(Renderer::ScreenOrientation orientation);
    static Renderer::ScreenOrientation GetOrientation();
};

}

#endif //A2D_NATIVE_TO_PLATFORM_H
