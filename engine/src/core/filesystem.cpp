//
// Created by selya on 23.11.2018.
//

#include <a2d/core/filesystem.h>

#ifdef TARGET_ANDROID
#include <android/asset_manager.h>
#endif

namespace a2d {

#ifdef TARGET_ANDROID
AAssetManager *a2d::FileSystem::asset_manager = nullptr;
#endif

std::vector<unsigned char> FileSystem::LoadRaw(const std::string &path) {
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

std::string FileSystem::LoadText(const std::string &path) {
    auto v = LoadRaw(path);
    return std::string(v.begin(), v.end());
}

#ifdef TARGET_DESKTOP

bool FileSystem::Initialize() {
    return true;
}

#elif TARGET_ANDROID

bool FileSystem::Initialize(AAssetManager *asset_manager) {
    FileSystem::asset_manager = asset_manager;
    return true;
}

#endif

} //namespace a2d