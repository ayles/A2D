//
// Created by selya on 09.11.2018.
//

#include <a2d/core/components/sprite.hpp>
#include <a2d/graphics/renderer.hpp>
#include <a2d/core/object2d.hpp>

namespace a2d {

Sprite::Sprite() : color(1), shader(Shader::GetShader("default")), texture_region(nullptr) {

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

void Sprite::SetTextureRegion(const pTextureRegion &texture_region, bool pixel_size, bool adjust_size) {
    this->texture_region = texture_region;
    if (adjust_size) {
        if (pixel_size) {
            this->SetSize(float(texture_region->GetWidth()), float(texture_region->GetHeight()));
        } else {
            this->SetSize(1, texture_region->GetRatio());
        }
    }
}

void Sprite::Draw(SpriteBatch &sprite_batch) {
    static Vector2f p1, p2, p3, p4;
    p1.Set(-size.x * origin.x, -size.y * origin.y);
    p2.Set(size.x * (1 - origin.x), p1.y);
    p3.Set(p2.x, size.y * (1 - origin.y));
    p4.Set(p1.x, p3.y);
    sprite_batch.Draw(texture_region, shader, p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), color);
}

void Sprite::OnEnable() {

}

void Sprite::OnDisable() {

}

Sprite::~Sprite() {

}

} //namespace a2d
