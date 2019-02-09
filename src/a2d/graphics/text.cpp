//
// Created by selya on 05.12.2018.
//

#include <a2d/graphics/text.hpp>
#include <a2d/core/engine.hpp>

namespace a2d {


Text::Text() : text(), bitmap_font(nullptr), text_width(0), color(1) {}

void Text::SetText(const std::string &text) {
    ASSERT_MAIN_THREAD
    SetText(std::u32string(text.begin(), text.end()));
}

void Text::SetText(const std::u32string &text) {
    ASSERT_MAIN_THREAD
    this->text = text;
    text_width = 0;
    for (char32_t i : text) {
        text_width += bitmap_font->GetCharacter(i)->advance_x;
    }
    size.Set(text_width, (float)bitmap_font->GetLineHeight());
}

void Text::SetFont(const pBitmapFont &bitmap_font) {
    ASSERT_MAIN_THREAD
    this->bitmap_font = bitmap_font;
}

const std::u32string &Text::GetUTF32Text() const {
    ASSERT_MAIN_THREAD
    return text;
}

std::string Text::GetText() const {
    ASSERT_MAIN_THREAD
    return std::string(text.begin(), text.end());
}

pBitmapFont &Text::GetFont() {
    ASSERT_MAIN_THREAD
    return bitmap_font;
}

void Text::Draw(SpriteBatch &sprite_batch) {
    static Vector2f p1, p2, p3, p4;
    float x1 = -size.x * origin.x;
    float y1 = -size.y * origin.y;
    float current_x = 0.0f;
    for (auto c : text) {
        auto t = bitmap_font->GetCharacter(c);
        if (!t || !t->texture_region) continue;
        float x = x1 + current_x + t->x;
        float y = y1 + t->y;
        p1.Set(x, y);
        p2.Set(x + t->texture_region->GetWidth(), y);
        p3.Set(x + t->texture_region->GetWidth(), y + t->texture_region->GetHeight()
        );
        p4.Set(x, y + t->texture_region->GetHeight());
        sprite_batch.Draw(t->texture_region, shader, p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), color);
        current_x += t->advance_x;
    }
}

Text::~Text() = default;


} //namespace a2d
