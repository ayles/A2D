//
// Created by selya on 09.11.2018.
//

#include <A2D/core/components/sprite.h>
#include <A2D/renderer.h>

namespace a2d {

Sprite::Sprite() : color(1) {

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
