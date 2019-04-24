//
// Created by selya on 25.11.2018.
//

#ifndef A2D_BITMAP_FONT_H
#define A2D_BITMAP_FONT_H

#include <a2d/core/macro.hpp>
#include <a2d/renderer/texture/texture_region.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/ref_counter.hpp>
#include <a2d/renderer/texture/texture_atlas.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#include <vector>

namespace a2d {

DECLARE_SMART_POINTER(BitmapFont)

class BitmapFont : public ref_counter {
public:
    class Character {
        friend class BitmapFont;

    public:
        pTextureRegion texture_region;
        const float x, y;
        const float advance_x;
        const float advance_y;

    private:
        Character(const pTextureRegion &texture_region, float x, float y, float advance_x, float advance_y);
    };

    class CharacterSetParams {
        friend class BitmapFont;

        const int size;
        const int outline_size_64;

        CharacterSetParams(int size, int outline_size_64);

    public:
        CharacterSetParams(const CharacterSetParams &other);

        bool operator<(const CharacterSetParams &other) const;
    };

    class CharacterSet {
        friend class BitmapFont;

        std::map<char32_t, Character> characters;
        float line_height;
        pTextureAtlas texture_atlas;
        BitmapFont *bitmap_font;
        CharacterSetParams params;

        CharacterSet(BitmapFont *bitmap_font, const CharacterSetParams &params);
        const Character *RenderAndStoreCharacter(char32_t char_code);

    public:
        float GetLineHeight() const;
        const Character *GetCharacter(char32_t char_code) const;
        pTexture GetTexture() const;
    };

private:
    std::map<CharacterSetParams, CharacterSet> characters_sets;
    FT_Face face;
    std::vector<unsigned char> data;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(BitmapFont)

    CharacterSet &GetCharacterSet(float size, float outline_size = 0.0f);

    static pBitmapFont Create(const std::vector<unsigned char> &ttf);

private:
    BitmapFont(const std::vector<unsigned char> &ttf);
    ~BitmapFont() override;

    static FT_Library GetFreeTypeLibrary();
};

} //namespace a2d

#endif //A2D_BITMAP_FONT_H
