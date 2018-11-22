//
// Created by selya on 09.11.2018.
//

#include <a2d/core/components/sprite.h>
#include <a2d/core/renderer.h>

namespace a2d {

Sprite::Sprite() : color(1), texture_region(nullptr), shader(Shader::GetShader("default")) {

}

const pTextureRegion &Sprite::GetTextureRegion() {
    return texture_region;
}

void Sprite::SetTextureRegion(const pTextureRegion &texture_region) {
    this->texture_region = texture_region;
}

void Sprite::OnEnable() {
    Renderer::AddSprite(this);
}

void Sprite::OnDisable() {
    Renderer::RemoveSprite(this);
}

void Sprite::LayerHierarchyAboutToChange() {
    if (IsActive()) {
        Renderer::RemoveSprite(this);
    }
}

void Sprite::LayerHierarchyChanged() {
    if (IsActive()) {
        Renderer::AddSprite(this);
    }
}

Sprite::~Sprite() {

}

} //namespace a2d
