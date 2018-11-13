//
// Created by selya on 12.11.2018.
//

#ifndef A2D_FILESYSTEM_H
#define A2D_FILESYSTEM_H

#include <a2d/core/native_filesystem.h>

#include <string>
#include <vector>

#ifdef TARGET_ANDROID
#include <android/asset_manager.h>
#endif

namespace a2d {

class FileSystem {
public:
    friend class NativeFileSystem;

    static std::vector<unsigned char> LoadRaw(const std::string &path) {
        return NativeFileSystem::LoadRaw(path);
    }
};

} //namespace a2d

#endif //A2D_FILESYSTEM_H
