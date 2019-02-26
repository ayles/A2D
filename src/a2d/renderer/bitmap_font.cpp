//
// Created by selya on 28.11.2018.
//

#include <a2d/renderer/bitmap_font.hpp>
#include <a2d/renderer/gl.hpp>
#include <a2d/core/log.hpp>


namespace a2d {

BitmapFont::Character::Character(const a2d::pTextureRegion &texture_region,
        float x, float y, float advance_x, float advance_y) :
texture_region(texture_region), x(x), y(y), advance_x(advance_x), advance_y(advance_y) {
    ASSERT_MAIN_THREAD
}

float BitmapFont::CharacterSet::GetLineHeight() const {
    ASSERT_MAIN_THREAD
    return line_height;
}

const BitmapFont::Character *BitmapFont::CharacterSet::GetCharacter(char32_t char_code) const {
    ASSERT_MAIN_THREAD
    auto i = characters.find(char_code);
    if (i == characters.end()) {
        LOG_TRACE("Can't find character");
        return nullptr;
    }
    return &i->second;
}

pTexture BitmapFont::CharacterSet::GetTexture() const {
    ASSERT_MAIN_THREAD
    return texture;
}

BitmapFont::CharacterSet::CharacterSet() : line_height(0) {}

BitmapFont::CharacterSetParams::CharacterSetParams(int size, int outline_size_64) :
size(size), outline_size_64(outline_size_64) {}

bool BitmapFont::CharacterSetParams::operator<(const BitmapFont::CharacterSetParams &other) const {
    if (size != other.size) return size < other.size;
    return outline_size_64 < other.outline_size_64;
}

BitmapFont::CharacterSetParams::CharacterSetParams(const BitmapFont::CharacterSetParams &other) :
size(other.size), outline_size_64(other.outline_size_64) {}

BitmapFont::CharacterSet &BitmapFont::GetCharacterSet(float size, float outline_size) {
    CharacterSetParams params(int(size), int(outline_size * 64));
    auto iter = characters_sets.find(params);
    if (iter != characters_sets.end()) return iter->second;
    return RenderAndStoreCharacterSet(params);
}

BitmapFont::CharacterSet &BitmapFont::RenderAndStoreCharacterSet(const CharacterSetParams &params) {
    FT_Set_Pixel_Sizes(face, 0, (FT_UInt)params.size);

    auto &c_set = characters_sets.emplace(params, CharacterSet()).first->second;

    c_set.line_height = (float)face->size->metrics.height / 64;

    int max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

    int texture_width = 0;
    int texture_height = 0;

    int current_x = 0;
    int max_height = 0;

    FT_Stroker stroker;
    FT_Stroker_New(GetFreeTypeLibrary(), &stroker);
    FT_Stroker_Set(stroker, params.outline_size_64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

    // Calculate texture size
    unsigned int glyph_index;
    auto char_code = FT_Get_First_Char(face, &glyph_index);
    FT_GlyphSlot g = face->glyph;
    while (glyph_index) {
        FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);

        FT_Glyph glyph;
        FT_Get_Glyph(g, &glyph);
        FT_Glyph_StrokeBorder(&glyph, stroker, false, true);
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
        auto bitmap_glyph = (FT_BitmapGlyph)glyph;
        auto &bitmap = bitmap_glyph->bitmap;

        max_height = max_height > bitmap.rows ? max_height : bitmap.rows;
        if (current_x + bitmap.width > max_texture_size) {
            texture_height += max_height;
            current_x = 0;
            max_height = 0;
        }
        current_x += bitmap.width;
        texture_width = texture_width > current_x ? texture_width : current_x;
        char_code = FT_Get_Next_Char(face, char_code, &glyph_index);
    }
    texture_height += max_height;

    texture_width = (int)std::pow(2, std::ceil(std::log2(texture_width)));
    texture_height = (int)std::pow(2, std::ceil(std::log2(texture_height)));

    c_set.texture = Texture::Create(texture_width, texture_height);
    TextureBuffer &tb = c_set.texture->GetBuffer();

    max_height = 0;
    current_x = 0;
    int current_y = 0;

    char_code = FT_Get_First_Char(face, &glyph_index);
    while (glyph_index) {
        FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        //FT_Render_Glyph(g, FT_RENDER_MODE_NORMAL);

        FT_Glyph glyph;
        FT_Get_Glyph(g, &glyph);
        FT_Glyph_StrokeBorder(&glyph, stroker, false, true);
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
        auto bitmap_glyph = (FT_BitmapGlyph)glyph;
        auto &bitmap = bitmap_glyph->bitmap;
        //auto &bitmap = g->bitmap;

        max_height = max_height > bitmap.rows ? max_height : bitmap.rows;
        if (current_x + bitmap.width > max_texture_size) {
            current_y += max_height;
            current_x = 0;
            max_height = 0;
        }

        for (int x = 0; x < (int)bitmap.width; ++x) {
            for (int y = 0; y < (int)bitmap.rows; ++y) {
                unsigned char value = bitmap.buffer[(bitmap.rows - y - 1) * bitmap.width + x];
                tb.SetPixel(
                        current_x + x, current_y + y,
                        255, 255, 255, value
                );
            }
        }

        c_set.characters.emplace(char_code, Character(
                TextureRegion::Create(
                        c_set.texture,
                        current_x, current_y,
                        bitmap.width, bitmap.rows
                ),
                g->bitmap_left - (float)params.outline_size_64 / 64,
                g->bitmap_top - g->bitmap.rows - (float)params.outline_size_64 / 64,
                (float)g->advance.x / 64,
                (float)g->advance.y / 64
        ));

        current_x += bitmap.width;

        char_code = FT_Get_Next_Char(face, char_code, &glyph_index);
    }

    return c_set;
}

BitmapFont::BitmapFont(const std::vector<unsigned char> &ttf) : data(ttf) {
    ASSERT_MAIN_THREAD
    FT_New_Memory_Face(GetFreeTypeLibrary(), &data[0], data.size(), 0, &face);
}

BitmapFont::~BitmapFont() {
    ASSERT_MAIN_THREAD
    FT_Done_Face(face);
}

FT_Library BitmapFont::GetFreeTypeLibrary() {
    ASSERT_MAIN_THREAD
    static FT_Library ft_library;
    if (!ft_library) {
        FT_Init_FreeType(&ft_library);
    }
    return ft_library;
}

pBitmapFont BitmapFont::Create(const std::vector<unsigned char> &ttf) {
    ASSERT_MAIN_THREAD
    return new BitmapFont(ttf);
}

} //namespace a2d

