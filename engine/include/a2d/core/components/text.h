//
// Created by selya on 25.11.2018.
//

#ifndef A2D_TEXT_H
#define A2D_TEXT_H

#include <a2d/core/drawable.h>
#include <string>
#include <a2d/core/bitmap_font.h>
#include <a2d/core/object2d.h>

#include <codecvt>

namespace a2d {

class Text : public Drawable {
protected:
    std::u32string text;
    pBitmapFont bitmap_font = nullptr;
    pShader shader = Shader::GetShader("default");

public:
    Vector4f color;

    void SetText(const std::string &text) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
        this->text = convert.from_bytes(text);
    }

    void SetText(const std::u32string &text) {
        this->text = text;
    }

    void SetFont(const pBitmapFont &bitmap_font) {
        this->bitmap_font = bitmap_font;
    }

    const std::u32string &GetText() const {
        return text;
    }

    pBitmapFont &GetFont() {
        return bitmap_font;
    }

    void Draw(SpriteBatch &sprite_batch) override {
        static Vector2f p1, p2, p3, p4;
        float current_x = 0.0f;
        for (auto c : text) {
            const auto &t = bitmap_font->GetCharacter(c);
            if (!t) continue;
            p1.Set(current_x + 0.0f, -1.0f);
            p2.Set(current_x + t->ratio, -1.0f);
            p3.Set(current_x + t->ratio, 3.0f);
            p4.Set(current_x + 0.0f, 3.0f);
            sprite_batch.Draw(t->texture_region, shader, p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), color);
            current_x += t->ratio;
        }
    }
};

} //namespace a2d

#endif //A2D_TEXT_H
