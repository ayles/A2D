//
// Created by selya on 08.01.2019.
//

#include <a2d/core/resources.hpp>
#include <a2d/graphics/shader.hpp>
#include <a2d/filesystem/filesystem.hpp>

namespace a2d {

template<>
intrusive_ptr<Shader> Resources::Load(const std::string &name) {
    return new Shader(
            FileSystem::LoadText("shaders/" + name + "vertex.glsl"),
            FileSystem::LoadText("shaders/" + name + "/fragment.glsl")
    );
}

} //namespace a2d
