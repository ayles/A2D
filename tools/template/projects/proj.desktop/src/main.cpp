//
// Created by selya on 05.11.2018.
//

#include <a2d/native_bridge.hpp>

int main() {
    if (!a2d::NativeBridge::Initialize()) return -1;
    while (a2d::NativeBridge::Step());
    a2d::NativeBridge::Uninitialize();

    return 0;
}
