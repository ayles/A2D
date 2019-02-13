//
// Created by selya on 25.11.2018.
//

#ifndef A2D_TEXT_H
#define A2D_TEXT_H

#include <a2d/renderer/drawable.hpp>
#include <string>
#include <a2d/renderer/bitmap_font.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/resources.hpp>

#include <codecvt>

namespace a2d {

class Text : public Drawable {
    friend class Object2D;

protected:
    std::u32string text;
    pBitmapFont bitmap_font;
    float text_width;

public:
    Vector4f color;

    void SetText(const std::string &text);
    void SetText(const std::u32string &text);

    void SetFont(const pBitmapFont &bitmap_font);

    const std::u32string &GetUTF32Text() const;
    std::string GetText() const;

    pBitmapFont &GetFont();

protected:
    Text();
    ~Text() override;

    void Draw(SpriteBatch &sprite_batch) override;
};

} //namespace a2d

#endif //A2D_TEXT_H
