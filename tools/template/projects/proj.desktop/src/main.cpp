//
// Created by selya on 05.11.2018.
//

#include <a2d/native_bridge.hpp>

int main() {
#ifdef TARGET_LINUX
    setenv("DISPLAY", ":0", true);
    setenv("LIBGL_ALWAYS_INDIRECT", "1", true);
#endif

    if (!a2d::NativeBridge::Initialize()) return -1;
    while (a2d::NativeBridge::Step());
    a2d::NativeBridge::Uninitialize();

    return 0;
}