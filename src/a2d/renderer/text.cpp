//
// Created by selya on 05.12.2018.
//

#include <a2d/renderer/text.hpp>
#include <a2d/core/engine.hpp>

namespace a2d {


Text::Text() : text(), bitmap_font(nullptr),
outline_material(Material::Create(Resources::Get<Shader>("default"))),
font_size(0), outline_width(0),
color(1) {}

void Text::SetColor(const Vector4f &color) {
    SetColor(color.x, color.y, color.z, color.w);
}

void Text::SetColor(float r, float g, float b, float a) {
    this->color.Set(r, g, b, a);
}

void Text::SetOutlineColor(const Vector4f &outline_color) {
    SetColor(outline_color.x, outline_color.y, outline_color.z, outline_color.w);
}

void Text::SetOutlineColor(float r, float g, float b, float a) {
    this->outline_color.Set(r, g, b, a);
}

void Text::SetFontSize(float font_size) {
    this->font_size = font_size;
    InternalUpdate();
}

void Text::SetOutlineWidth(float outline_width) {
    this->outline_width = outline_width;
    InternalUpdate();
}

void Text::SetText(const std::string &text) {
    ASSERT_MAIN_THREAD
    SetText(std::u32string(text.begin(), text.end()));
}

void Text::SetText(const std::u32string &text) {
    ASSERT_MAIN_THREAD
    this->text = text;
    InternalUpdate();
}

void Text::SetFont(const pBitmapFont &bitmap_font) {
    ASSERT_MAIN_THREAD
    this->bitmap_font = bitmap_font;
    InternalUpdate();
}

Vector4f a2d::Text::GetColor() const {
    return color;
}

Vector4f a2d::Text::GetOutlineColor() const {
    return outline_color;
}

float a2d::Text::GetOutlineWidth() const {
    return outline_width;
}

float Text::GetFontSize() const {
    return font_size;
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

void Text::InternalUpdate() {
    float text_width = 0.0f;
    auto &c_set = bitmap_font->GetCharacterSet(font_size);
    for (char32_t i : text) {
        text_width += c_set.GetCharacter(i)->advance_x;
    }
    size.Set(text_width, c_set.GetLineHeight());
    if (this->material) {
        auto &c_set2 = bitmap_font->GetCharacterSet(font_size, outline_width);
        this->material->SetTexture("main_texture", c_set.GetTexture());
        this->outline_material->SetTexture("main_texture", c_set2.GetTexture());
    }
}

void Text::Draw(SpriteBatch &sprite_batch) {
    Vector2f p1, p2, p3, p4;
    float x1 = -size.x * origin.x;
    float y1 = -size.y * origin.y;
    float current_x = 0.0f;
    auto &c_set = bitmap_font->GetCharacterSet(font_size, outline_width);
    for (auto c : text) {
        auto t = c_set.GetCharacter(c);
        if (!t || !t->texture_region) continue;
        float x = x1 + current_x + t->x;
        float y = y1 + t->y;
        p1.Set(x, y);
        p2.Set(x + t->texture_region->GetWidth(), y);
        p3.Set(x + t->texture_region->GetWidth(), y + t->texture_region->GetHeight());
        p4.Set(x, y + t->texture_region->GetHeight());
        sprite_batch.Draw(outline_material,
                t->texture_region->GetUVLower(), t->texture_region->GetUVUpper(),
                p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), outline_color);
        current_x += t->advance_x;
    }

    x1 = -size.x * origin.x;
    y1 = -size.y * origin.y;
    current_x = 0.0f;
    auto &c_set2 = bitmap_font->GetCharacterSet(font_size);
    for (auto c : text) {
        auto t = c_set2.GetCharacter(c);
        if (!t || !t->texture_region) continue;
        float x = x1 + current_x + t->x;
        float y = y1 + t->y;
        p1.Set(x, y);
        p2.Set(x + t->texture_region->GetWidth(), y);
        p3.Set(x + t->texture_region->GetWidth(), y + t->texture_region->GetHeight());
        p4.Set(x, y + t->texture_region->GetHeight());
        sprite_batch.Draw(material,
                          t->texture_region->GetUVLower(), t->texture_region->GetUVUpper(),
                          p1, p2, p3, p4, GetObject2D()->GetTransformMatrix(), color);
        current_x += t->advance_x;
    }
}

Text::~Text() = default;


} //namespace a2d
