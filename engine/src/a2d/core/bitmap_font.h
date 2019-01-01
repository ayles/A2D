//
// Created by selya on 25.11.2018.
//

#ifndef A2D_BITMAP_FONT_H
#define A2D_BITMAP_FONT_H

#include <a2d/core/macro.h>
#include <a2d/graphics/texture_region.h>
#include <a2d/core/engine.h>
#include <a2d/core/ref_counter.h>

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
        int width, height;
        float ratio;

        Character();
        Character(const pTextureRegion &texture_region, int x, int y, int width, int height);
    };

private:
    std::map<unsigned long, Character> characters;
    int line_height;

public:
    const Character *GetCharacter(unsigned long char_code) const;
    int GetLineHeight() const;

    BitmapFont(const std::vector<unsigned char> &ttf, int size);
    ~BitmapFont() override;

private:
    static FT_Library GetFreeTypeLibrary();
};

} //namespace a2d

#endif //A2D_BITMAP_FONT_H
