//
// Created by selya on 05.12.2018.
//

#include <a2d/core/components/text.h>

namespace a2d {


Text::Text() : text(), bitmap_font(nullptr), shader(Shader::GetShader("default")), color(1) {}

void Text::SetText(const std::string &text) {
    // TODO fix Visual Studio bug
    // std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    // SetText(convert.from_bytes(text));
    std::u32string s;
    s.reserve(text.length());
    for (int i = 0; i < (int)text.length(); ++i) {
        s[i] = text[i];
    }
    SetText(s);
}

void Text::SetText(const std::u32string &text) {
    this->text = text;
    float width = 0;
    for (char32_t i : text) {
        width += bitmap_font->GetCharacter(i)->width;
    }
    size.Set(width, (float)bitmap_font->GetLineHeight());
}

void Text::SetFont(const pBitmapFont &bitmap_font) {
    this->bitmap_font = bitmap_font;
}

const std::u32string &Text::GetUTF32Text() const {
    return text;
}

std::string Text::GetText() const {
    // TODO fix bug
    // std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    // return convert.to_bytes(text);
    std::string s;
    s.reserve(text.length());
    for (int i = 0; i < (int)text.length(); ++i) {
        s[i] = (char)text[i];
    }
    return s;
}

pBitmapFont &Text::GetFont() {
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
        p1.Set(x1 + current_x, y1 - t->y);
        p2.Set(x1 + current_x + t->texture_region->GetWidth(), y1 - t->y);
        p3.Set(x1 + current_x + t->texture_region->GetWidth(), y1 - t->y + t->texture_region->GetHeight());
        p4.Set(x1 + current_x, y1 - t->y + t->texture_region->GetHeight());
        sprite_batch.Draw(t->texture_region, shader, p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), color);
        current_x += t->texture_region->GetWidth();
    }
}

Text::~Text() = default;


} //namespace a2d
