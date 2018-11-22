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

    static std::vector<unsigned char> LoadRaw(const std::string &path) {
#ifdef TARGET_ANDROID
        auto asset = AAssetManager_open(asset_manager, path.c_str(), AASSET_MODE_UNKNOWN);
        auto size = AAsset_getLength(asset);
        std::vector<unsigned char> v(size);
        AAsset_read (asset, &v[0], size);
        AAsset_close(asset);
        return v;
#elif TARGET_DESKTOP
        std::ifstream file("resources/" + path, std::ios::binary);
        if ((file.rdstate() & std::ifstream::failbit) != 0) {
            std::vector<unsigned char> a;
            return a;
        }
        return std::vector<unsigned char>(
                (std::istreambuf_iterator<char>(file)),
                (std::istreambuf_iterator<char>())
        );
#endif
    }

    static std::string LoadText(const std::string &path) {
        auto v = LoadRaw(path);
        return std::string(v.begin(), v.end());
    }

private:
#ifdef TARGET_DESKTOP
    static bool Initialize() {
        return true;
    }
#elif TARGET_ANDROID
    private:
    static AAssetManager *asset_manager;
public:
    static bool Initialize(AAssetManager *asset_manager) {
        FileSystem::asset_manager = asset_manager;
        return true;
    }
#endif

    static void Uninitialize() {}
};

} //namespace a2d

#endif //A2D_FILESYSTEM_H
