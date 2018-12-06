//
// Created by selya on 12.11.2018.
//

#ifndef A2D_FILESYSTEM_H
#define A2D_FILESYSTEM_H

#include <a2d/core/engine.h>

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#ifdef TARGET_ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

namespace a2d {

class FileSystem {
public:
    friend class NativeConnector;

    static std::vector<unsigned char> LoadRaw(const std::string &path);
    static std::string LoadText(const std::string &path);

private:
#ifdef TARGET_DESKTOP
    static bool Initialize();
#elif TARGET_ANDROID
    static AAssetManager *asset_manager;

    // TODO move to private section
public:
    static bool Initialize(AAssetManager *asset_manager);
private:
#endif

    static void Uninitialize() {}
};

} //namespace a2d

#endif //A2D_FILESYSTEM_H
