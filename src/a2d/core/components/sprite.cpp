//
// Created by selya on 09.11.2018.
//

#include <a2d/core/components/sprite.hpp>
#include <a2d/core/object2d.hpp>

namespace a2d {

Sprite::Sprite() : color(1), scale_factor(1.0f) {}

float Sprite::GetScaleFactor() const {
    return scale_factor;
}

void Sprite::SetScaleFactor(float scale_factor) {
    this->scale_factor = scale_factor;
    if (texture_region) SetSize(texture_region->GetWidth() * scale_factor, texture_region->GetHeight() * scale_factor);
}

void Sprite::SetTextureRegion(const pTextureRegion &texture_region) {
    Drawable::SetTextureRegion(texture_region);
    if (!texture_region) SetSize(0.0f);
    this->texture_region = texture_region;
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
