//
// Created by selya on 09.11.2018.
//

#include <a2d/core/components/sprite.hpp>
#include <a2d/graphics/renderer.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/resources.hpp>

namespace a2d {

Sprite::Sprite() : color(1), shader(Resources::Get<Shader>("default")), texture_region(nullptr), scale_factor(1.0f) {}

float Sprite::GetScaleFactor() const {
    return scale_factor;
}

void Sprite::SetScaleFactor(float scale_factor) {
    this->scale_factor = scale_factor;
    if (texture_region) SetSize(texture_region->GetWidth() * scale_factor, texture_region->GetHeight() * scale_factor);
}

const pTextureRegion &Sprite::GetTextureRegion() {
    return texture_region;
}

const Shader *Sprite::GetShaderForSortOrNull() const {
    return shader.get();
}

const TextureRegion *Sprite::GetTextureRegionForSortOrNull() const {
    return texture_region.get();
}

void Sprite::SetTextureRegion(const pTextureRegion &texture_region, float scale_factor) {
    if (!texture_region) SetSize(0.0f);
    this->texture_region = texture_region;
    if (scale_factor == 0.0f) scale_factor = this->scale_factor;
    SetScaleFactor(scale_factor);
}

void Sprite::Draw(SpriteBatch &sprite_batch) {
    static Vector2f p1, p2, p3, p4;
    p1.Set(-size.x * origin.x, -size.y * origin.y);
    p2.Set(size.x * (1 - origin.x), p1.y);
    p3.Set(p2.x, size.y * (1 - origin.y));
    p4.Set(p1.x, p3.y);
    sprite_batch.Draw(texture_region, shader, p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), color);
}

Sprite::~Sprite() {

}

} //namespace a2d
