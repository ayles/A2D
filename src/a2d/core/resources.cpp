//
// Created by selya on 08.01.2019.
//

#include <a2d/core/resources.hpp>
#include <a2d/renderer/shader.hpp>
#include <a2d/filesystem/filesystem.hpp>
#include <a2d/renderer/texture.hpp>
#include <a2d/renderer/bitmap_font.hpp>

#include <lodepng.h>

namespace a2d {

template<>
intrusive_ptr<Shader> Resources::Load(const std::string &name) {
    ASSERT_MAIN_THREAD
    return Shader::Create(
            FileSystem::LoadText("shaders/" + name + "/vertex.glsl"),
            FileSystem::LoadText("shaders/" + name + "/fragment.glsl")
    );
}

template<>
intrusive_ptr<Texture> Resources::Load(const std::string &name) {
    ASSERT_MAIN_THREAD
    std::vector<unsigned char> raw_data = FileSystem::LoadRaw("textures/" + name + ".png");
    unsigned int width, height;
    std::vector<unsigned char> image;
    lodepng::decode(image, width, height, raw_data);

    auto texture = Texture::Create(width, height, image);
    texture->GetBuffer().FlipVertically();

    return texture;
}

template<>
intrusive_ptr<BitmapFont> Resources::Load(const std::string &name) {
    ASSERT_MAIN_THREAD
    return BitmapFont::Create(FileSystem::LoadRaw("fonts/" + name + ".ttf"));
}

} //namespace a2d
