//
// Created by selya on 25.11.2018.
//

#ifndef A2D_TEXT_HPP
#define A2D_TEXT_HPP

#include <a2d/renderer/drawable.hpp>
#include <a2d/core/object2d.hpp>

#include <string>
#include <codecvt>

namespace a2d {

class BitmapFont;

class Text : public Drawable {
    friend class Object2D;

protected:
    std::u32string text;
    intrusive_ptr<BitmapFont> bitmap_font;
    intrusive_ptr<Material> outline_material;
    float font_size;
    float outline_width;
    Vector4f color;
    Vector4f outline_color;

public:

    void SetColor(const Vector4f &color);
    void SetColor(float r, float g, float b, float a);
    void SetOutlineColor(const Vector4f &outline_color);
    void SetOutlineColor(float r, float g, float b, float a);
    void SetFontSize(float font_size);
    void SetOutlineWidth(float outline_width);
    void SetText(const std::string &text);
    void SetText(const std::u32string &text);
    void SetFont(const intrusive_ptr<BitmapFont> &bitmap_font);

    Vector4f GetColor() const;
    Vector4f GetOutlineColor() const;
    float GetFontSize() const;
    float GetOutlineWidth() const;

    const std::u32string &GetUTF32Text() const;
    std::string GetText() const;

    intrusive_ptr<BitmapFont> &GetFont();

protected:
    Text();
    ~Text() override;

    void Draw(SpriteBatch &sprite_batch, const Vector4f &color) override;
    void InternalUpdate();
};

} //namespace a2d

#endif //A2D_TEXT_HPP
