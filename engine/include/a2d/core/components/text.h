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

    Text();

    void SetText(const std::string &text);
    void SetText(const std::u32string &text);

    void SetFont(const pBitmapFont &bitmap_font);

    const std::u32string &GetUTF32Text() const;
    std::string GetText() const;

    pBitmapFont &GetFont();

    void Draw(SpriteBatch &sprite_batch) override;

    ~Text() override;
};

} //namespace a2d

#endif //A2D_TEXT_H
