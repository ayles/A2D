//
// Created by selya on 08.01.2019.
//

#include <a2d/core/resources.hpp>
#include <a2d/graphics/shader.hpp>
#include <a2d/filesystem/filesystem.hpp>
#include <a2d/graphics/texture.hpp>

#include <lodepng.h>

namespace a2d {

template<>
intrusive_ptr<Shader> Resources::Load(const std::string &name) {
    return new Shader(
            FileSystem::LoadText("shaders/" + name + "/vertex.glsl"),
            FileSystem::LoadText("shaders/" + name + "/fragment.glsl")
    );
}

template<>
intrusive_ptr<Texture> Resources::Load(const std::string &name) {
    std::vector<unsigned char> raw_data = FileSystem::LoadRaw("textures/" + name + ".png");
    unsigned int width, height;
    std::vector<unsigned char> image;
    lodepng::decode(image, width, height, raw_data);

    auto texture = new Texture(width, height, &image[0]);
    texture->buffer.FlipVertically();

    return texture;
}

} //namespace a2d
