//
// Created by selya on 28.11.2018.
//

#include <a2d/graphics/bitmap_font.hpp>
#include <a2d/graphics/gl.hpp>
#include <a2d/core/log.hpp>


namespace a2d {

BitmapFont::Character::Character() :
texture_region(nullptr), x(0), y(0), advance_x(0), advance_y(0) {
    ASSERT_MAIN_THREAD
}

BitmapFont::Character::Character(const a2d::pTextureRegion &texture_region, int x, int y, int advance_x, int advance_y) :
texture_region(texture_region), x(x), y(y), advance_x(advance_x), advance_y(advance_y) {
    ASSERT_MAIN_THREAD
}

const BitmapFont::Character *BitmapFont::GetCharacter(unsigned long char_code) const {
    ASSERT_MAIN_THREAD
    auto i = characters.find(char_code);
    if (i == characters.end()) {
        LOG_TRACE("Can't find character");
        return nullptr;
    }
    return &i->second;
}

int BitmapFont::GetLineHeight() const {
    ASSERT_MAIN_THREAD
    return line_height;
}

BitmapFont::BitmapFont(const std::vector<unsigned char> &ttf, int size) {
    ASSERT_MAIN_THREAD
    FT_Face face;
    FT_New_Memory_Face(GetFreeTypeLibrary(), &ttf[0], ttf.size(), 0, &face);
    FT_Set_Pixel_Sizes(face, 0, (FT_UInt)size);

    int row_height = (int)std::ceil((float)face->size->metrics.height / 64);

    int max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

    int texture_width = 0;
    int texture_height = 0;

    int current_x = 0;
    int max_height = 0;

    // Calculate texture size
    unsigned int glyph_index;
    unsigned long char_code = FT_Get_First_Char(face, &glyph_index);
    FT_GlyphSlot g = face->glyph;
    while (glyph_index) {
        FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
        max_height = max_height > g->bitmap.rows ? max_height : g->bitmap.rows;
        current_x += g->bitmap.width;
        texture_width = texture_width > current_x ? texture_width : current_x;
        if (current_x + g->bitmap.width > max_texture_size) {
            texture_height += max_height;
            current_x = 0;
            max_height = 0;
        }
        char_code = FT_Get_Next_Char(face, char_code, &glyph_index);
    }
    texture_height += max_height;

    texture_width = (int)std::pow(2, std::ceil(std::log2(texture_width)));
    texture_height = (int)std::pow(2, std::ceil(std::log2(texture_height)));

    pTexture texture = Texture::Create(texture_width, texture_height);
    texture->buffer.Allocate(true);


    max_height = 0;
    current_x = 0;
    int current_y = 0;

    char_code = FT_Get_First_Char(face, &glyph_index);
    while (glyph_index) {
        FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);

        for (int x = 0; x < (int)g->bitmap.width; ++x) {
            for (int y = 0; y < (int)g->bitmap.rows; ++y) {
                unsigned char value = g->bitmap.buffer[(g->bitmap.rows - y - 1) * g->bitmap.width + x];
                texture->buffer.SetPixel(
                    current_x + x, current_y + y,
                    255, 255, 255, value
                );
            }
        }

        characters[char_code] = Character(
                TextureRegion::Create(
                        texture,
                        current_x, current_y,
                        g->bitmap.width, g->bitmap.rows,
                        Texture::Filtering::LINEAR
                ),
                g->bitmap_left,
                g->bitmap_top - g->bitmap.rows,
                g->advance.x / 64,
                g->advance.y / 64
        );

        max_height = max_height > g->bitmap.rows ? max_height : g->bitmap.rows;
        current_x += g->bitmap.width;
        if (current_x + g->bitmap.width > max_texture_size) {
            current_y += max_height;
            current_x = 0;
            max_height = 0;
        }

        char_code = FT_Get_Next_Char(face, char_code, &glyph_index);
    }

    line_height = row_height;
    FT_Done_Face(face);
}

BitmapFont::~BitmapFont() {
    ASSERT_MAIN_THREAD
    FT_Done_FreeType(GetFreeTypeLibrary());
}

FT_Library BitmapFont::GetFreeTypeLibrary() {
    ASSERT_MAIN_THREAD
    static FT_Library ft_library;
    if (!ft_library) {
        FT_Init_FreeType(&ft_library);
    }
    return ft_library;
}

pBitmapFont BitmapFont::Create(const std::vector<unsigned char> &ttf, int size) {
    ASSERT_MAIN_THREAD
    return new BitmapFont(ttf, size);
}

} //namespace a2d

