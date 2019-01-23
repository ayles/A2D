//
// Created by selya on 27.12.2018.
//

#include <a2d/filesystem/filesystem.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/log.hpp>

#ifdef TARGET_ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#elif TARGET_IOS
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <fstream>
#include <codecvt>

namespace a2d {

#ifdef TARGET_ANDROID
AAssetManager *asset_manager = nullptr;

void FileSystem::Initialize(void *asset_manager) {
    a2d::asset_manager = (AAssetManager *)asset_manager;
}
#endif

std::vector<unsigned char> FileSystem::LoadRaw(const std::string &path) {
    ASSERT_MAIN_THREAD
#ifdef TARGET_ANDROID
    auto asset = AAssetManager_open(asset_manager, path.c_str(), AASSET_MODE_UNKNOWN);
    ASSERT(asset != nullptr)
    auto size = AAsset_getLength(asset);
    std::vector<unsigned char> v(size);
    AAsset_read(asset, &v[0], size);
    AAsset_close(asset);
    return v;
#elif TARGET_IOS
    std::string path_processed;
    path_processed.resize(path.size());
    for (int i = 0; i < path.size(); ++i) {
        path_processed[i] = (path[i] == '/') ? '_' : path[i];
    }
    path_processed = "resources_" + path_processed;
    const char *c = path_processed.c_str();
    auto url = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFStringCreateWithCString(nullptr, c, kCFStringEncodingASCII), nullptr, nullptr);
    unsigned char fs_path[1024];
    CFURLGetFileSystemRepresentation(url, true, fs_path, sizeof(fs_path));
    std::ifstream file((char *)fs_path, std::ios::binary);
    if ((file.rdstate() & std::ifstream::failbit) != 0) {
        DEBUG_ERROR("Failed to load file")
        std::vector<unsigned char> a;
        return a;
    }
    return std::vector<unsigned char>(
            (std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>())
    );
#elif TARGET_DESKTOP
    std::ifstream file("resources/" + path, std::ios::binary);
    if ((file.rdstate() & std::ifstream::failbit) != 0) {
        LOG_TRACE("Failed to load file");
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
    ASSERT_MAIN_THREAD
    auto v = LoadRaw(path);
    return std::string(v.begin(), v.end());
}

std::u32string FileSystem::LoadTextUTF8(const std::string &path) {
    ASSERT_MAIN_THREAD
    // MSVC bug
    // https://stackoverflow.com/questions/32055357/visual-studio-c-2015-stdcodecvt-with-char16-t-or-char32-t
#if _MSC_VER >= 1900
    std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> converter;
    auto v = LoadRaw(path);
    auto s = converter.from_bytes(std::string(v.begin(), v.end()));
    return std::u32string(s.begin(), s.end());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    auto v = LoadRaw(path);
    return converter.from_bytes(std::string(v.begin(), v.end()));
#endif
}

} //namespace a2d
