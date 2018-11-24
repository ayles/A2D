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

void a2d::PixelSprite::Update() {
    GetObject2D()->scale.Set(texture_region ? scale_factor * texture_region->GetHeight() : 1);
    Sprite::Update();
}
