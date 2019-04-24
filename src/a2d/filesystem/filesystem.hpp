//
// Created by selya on 27.12.2018.
//

#ifndef A2D_FILESYSTEM_HPP
#define A2D_FILESYSTEM_HPP

#include <string>
#include <vector>

namespace a2d {

class FileSystem {
    friend class NativeBridge;

#if TARGET_ANDROID
    static void Initialize(void *asset_manager);
#endif

public:
    static std::vector<unsigned char> LoadRaw(const std::string &path);
    static std::string LoadText(const std::string &path);
    static std::u32string LoadTextUTF8(const std::string &path);
};

} //namespace a2d

#endif //A2D_FILESYSTEM_HPP
