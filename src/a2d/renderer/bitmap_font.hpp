//
// Created by selya on 25.11.2018.
//

#ifndef A2D_BITMAP_FONT_H
#define A2D_BITMAP_FONT_H

#include <a2d/core/macro.hpp>
#include <a2d/renderer/texture_region.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/ref_counter.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>

namespace a2d {

DECLARE_SMART_POINTER(BitmapFont)

class BitmapFont : public ref_counter {
public:
    class Character {
    public:
        pTextureRegion texture_region;
        int x, y;
        int advance_x;
        int advance_y;

        Character();
        Character(const pTextureRegion &texture_region, int x, int y, int advance_x, int advance_y);
    };

private:
    std::map<unsigned long, Character> characters;
    int line_height;

public:
    const Character *GetCharacter(unsigned long char_code) const;
    int GetLineHeight() const;

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(BitmapFont)

    static pBitmapFont Create(const std::vector<unsigned char> &ttf, int size, Texture::Filtering filtering = Texture::Filtering::NEAREST);

private:
    BitmapFont(const std::vector<unsigned char> &ttf, int size, Texture::Filtering filtering = Texture::Filtering::LINEAR);
    ~BitmapFont() override;

    static FT_Library GetFreeTypeLibrary();
};

} //namespace a2d

#endif //A2D_BITMAP_FONT_H
