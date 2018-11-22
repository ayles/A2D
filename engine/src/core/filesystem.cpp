//
// Created by selya on 23.11.2018.
//

#ifdef TARGET_ANDROID
#include <a2d/core/filesystem.h>

#include <android/asset_manager.h>

AAssetManager *a2d::FileSystem::asset_manager = nullptr;
#endif
