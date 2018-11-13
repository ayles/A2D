//
// Created by selya on 13.11.2018.
//

#include <a2d/core/native_filesystem.h>

#ifdef TARGET_ANDROID
AAssetManager *a2d::NativeFileSystem::asset_manager = nullptr;
#endif