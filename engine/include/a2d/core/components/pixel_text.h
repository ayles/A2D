//
// Created by selya on 27.11.2018.
//

#ifndef A2D_PIXEL_TEXT_H
#define A2D_PIXEL_TEXT_H

#include <a2d/core/components/text.h>

namespace a2d {

class PixelText : public Text {
public:
    PixelText() : scale_factor(1) {
        color.Set(1);
    }

    void SetScaleFactor(float scale_factor) {
        this->scale_factor = scale_factor;
    }

    float GetScaleFactor() {
        return scale_factor;
    }

    void Draw(SpriteBatch &sprite_batch) override {
        static Vector2f p1, p2, p3, p4;
        float current_x = 0.0f;
        for (auto c : text) {
            auto t = bitmap_font->GetCharacter(c);
            if (!t || !t->texture_region) continue;
            p1.Set(current_x + 0.0f, -t->x);
            p2.Set(current_x + t->texture_region->GetWidth() * scale_factor, -t->x);
            p3.Set(current_x + t->texture_region->GetWidth() * scale_factor, t->texture_region->GetHeight() * scale_factor);
            p4.Set(current_x + 0.0f, t->texture_region->GetHeight() * scale_factor);
            sprite_batch.Draw(t->texture_region, shader, p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), color);
            current_x += t->texture_region->GetWidth() * scale_factor;
        }
    }

private:
    float scale_factor;
};

} //namespace a2d

#endif //A2D_PIXEL_TEXT_H
