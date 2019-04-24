//
// Created by selya on 25.11.2018.
//

#ifndef A2D_BITMAP_FONT_H
#define A2D_BITMAP_FONT_H

#include <a2d/core/macro.hpp>
#include <a2d/core/ref_counter.hpp>

#include <vector>
#include <map>

namespace a2d {

class TextureAtlas;
class TextureRegion;
class Texture;

class BitmapFont : public ref_counter {
public:
    class Character {
        friend class BitmapFont;

    public:
        intrusive_ptr<TextureRegion> texture_region;
        const float x, y;
        const float advance_x;
        const float advance_y;

    private:
        Character(const intrusive_ptr<TextureRegion> &texture_region, float x, float y, float advance_x, float advance_y);
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
        intrusive_ptr<TextureAtlas> texture_atlas;
        BitmapFont *bitmap_font;
        CharacterSetParams params;

        CharacterSet(BitmapFont *bitmap_font, const CharacterSetParams &params);
        const Character *RenderAndStoreCharacter(char32_t char_code);

    public:
        float GetLineHeight() const;
        const Character *GetCharacter(char32_t char_code) const;
        intrusive_ptr<Texture> GetTexture() const;
    };

private:
    std::map<CharacterSetParams, CharacterSet> characters_sets;
    void *face;
    std::vector<unsigned char> data;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(BitmapFont)

    CharacterSet &GetCharacterSet(float size, float outline_size = 0.0f);

    static intrusive_ptr<BitmapFont> Create(const std::vector<unsigned char> &ttf);

private:
    BitmapFont(const std::vector<unsigned char> &ttf);
    ~BitmapFont() override;

    static void *GetFreeTypeLibrary();
};

} //namespace a2d

#endif //A2D_BITMAP_FONT_H
