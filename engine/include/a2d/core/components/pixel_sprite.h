//
// Created by selya on 23.11.2018.
//

#ifndef A2D_PIXEL_SPRITE_H
#define A2D_PIXEL_SPRITE_H

#include <a2d/core/components/sprite.h>

namespace a2d {

class PixelSprite : public Sprite {
public:
    PixelSprite();

    void SetScaleFactor(float scale_factor);
    float GetScaleFactor();

    void Draw(SpriteBatch &sprite_batch) override;

private:
    float scale_factor;
};

} //namespace a2d

#endif //A2D_PIXEL_SPRITE_H
