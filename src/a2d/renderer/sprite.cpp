//
// Created by selya on 09.11.2018.
//

#include <a2d/core/object2d.hpp>
#include <a2d/renderer/sprite.hpp>
#include <a2d/renderer/texture/texture_region.hpp>
#include <a2d/renderer/sprite_batch.hpp>

namespace a2d {

Sprite::Sprite() : color(1) {}


void a2d::Sprite::SetColor(const Vector4f &color) {
    SetColor(color.x, color.y, color.z, color.w);
}

void Sprite::SetColor(float r, float g, float b, float a) {
    color.Set(r, g, b, a);
}

void Sprite::SetTextureRegion(const intrusive_ptr<TextureRegion> &texture_region) {
    Drawable::SetTextureRegion(texture_region);
    if (!texture_region) SetSize(0.0f);
    else SetSize(texture_region->GetWidth(), texture_region->GetHeight());
}

const Vector4f &Sprite::GetColor() {
    return color;
}

void Sprite::Draw(SpriteBatch &sprite_batch, const Vector4f &color) {
    static Vector2f p1, p2, p3, p4;

    p1.Set(-size.x * origin.x, -size.y * origin.y);
    p2.Set(size.x * (1 - origin.x), p1.y);
    p3.Set(p2.x, size.y * (1 - origin.y));
    p4.Set(p1.x, p3.y);

    if (texture_region) {
        sprite_batch.Draw(material,
                          texture_region->GetUVLower(), texture_region->GetUVUpper(),
                          p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), this->color * color);
    } else {
        sprite_batch.Draw(material,
                          Vector2f(0), Vector2f(1),
                          p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), this->color * color);
    }
}

Sprite::~Sprite() {

}

} //namespace a2d
