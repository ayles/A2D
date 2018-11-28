//
// Created by selya on 23.11.2018.
//

#include <a2d/core/object2d.h>
#include <a2d/core/components/pixel_sprite.h>

a2d::PixelSprite::PixelSprite() : scale_factor(1) {

}

void a2d::PixelSprite::SetScaleFactor(float scale_factor) {
    this->scale_factor = scale_factor;
}

float a2d::PixelSprite::GetScaleFactor() {
    return scale_factor;
}

void a2d::PixelSprite::Draw(SpriteBatch &sprite_batch) {
    static Vector2f p1, p2, p3, p4;
    if (!texture_region) return;
    float width = std::abs(texture_region->GetWidth() * scale_factor);
    float height = std::abs(texture_region->GetHeight() * scale_factor);
    p1.Set(0.0f);
    p2.Set(width, 0.0f);
    p3.Set(width, height);
    p4.Set(0.0f, height);
    sprite_batch.Draw(texture_region, shader, p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), color);
}
