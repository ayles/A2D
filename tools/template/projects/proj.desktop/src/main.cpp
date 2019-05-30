//
// Created by selya on 05.11.2018.
//

#include <a2d/platform.h>

int main() {
    if (!a2d::PlatformToNative::Initialize()) return -1;
    while (a2d::PlatformToNative::Step());
    a2d::PlatformToNative::Uninitialize();

    return 0;
}
