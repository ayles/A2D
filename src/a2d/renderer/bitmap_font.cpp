//
// Created by selya on 28.11.2018.
//

#include <a2d/renderer/bitmap_font.hpp>
#include <a2d/renderer/gl.hpp>
#include <a2d/core/log.hpp>
#include <a2d/renderer/texture/texture_atlas.hpp>
#include <a2d/renderer/texture/texture_buffer.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

namespace a2d {

BitmapFont::Character::Character(const intrusive_ptr<TextureRegion> &texture_region,
        float x, float y, float advance_x, float advance_y) :
texture_region(texture_region), x(x), y(y), advance_x(advance_x), advance_y(advance_y) {}

float BitmapFont::CharacterSet::GetLineHeight() const {
    return line_height;
}

const BitmapFont::Character *BitmapFont::CharacterSet::GetCharacter(char32_t char_code) const {
    auto i = characters.find(char_code);
    if (i == characters.end()) {
        LOG_TRACE("Can't find character");
        return nullptr;
    }
    return &i->second;
}

intrusive_ptr<Texture> BitmapFont::CharacterSet::GetTexture() const {
    return texture_atlas->GetTexture();
}

BitmapFont::CharacterSet::CharacterSet(BitmapFont *bitmap_font, const CharacterSetParams &params) :
line_height(0), bitmap_font(bitmap_font), params(params) {
    int max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    texture_atlas = TextureAtlas::Create(max_texture_size);
    for (char32_t c = 32; c < 128; ++c) {
        RenderAndStoreCharacter(c);
    }
}

const BitmapFont::Character *BitmapFont::CharacterSet::RenderAndStoreCharacter(char32_t char_code) {
    FT_Set_Pixel_Sizes((FT_Face)bitmap_font->face, 0, (FT_UInt)params.size);
    line_height = (float)((FT_Face)bitmap_font->face)->size->metrics.height / 64;

    if (FT_Load_Char((FT_Face)bitmap_font->face, (FT_ULong)char_code, FT_LOAD_DEFAULT)) nullptr;

    FT_Stroker stroker;
    FT_Stroker_New((FT_Library)GetFreeTypeLibrary(), &stroker);
    FT_Stroker_Set(stroker, params.outline_size_64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

    FT_Glyph glyph;
    FT_Get_Glyph(((FT_Face)bitmap_font->face)->glyph, &glyph);
    FT_Glyph_StrokeBorder(&glyph, stroker, false, true);
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
    auto bitmap_glyph = (FT_BitmapGlyph)glyph;
    auto &bitmap = bitmap_glyph->bitmap;

    TextureBuffer tb(bitmap.width + 2, bitmap.rows + 2);

    for (int x = 0; x < (int)bitmap.width; ++x) {
        for (int y = 0; y < (int)bitmap.rows; ++y) {
            unsigned char value = bitmap.buffer[(bitmap.rows - y - 1) * bitmap.width + x];
            tb.SetPixel(x + 1, y + 1, 255, 255, 255, value);
        }
    }

    auto texture_region = texture_atlas->InsertTextureBuffer(tb);

    auto iter = characters.emplace(char_code, Character(
            texture_region,
            ((FT_Face)bitmap_font->face)->glyph->bitmap_left - (float)params.outline_size_64 / 64,
            ((FT_Face)bitmap_font->face)->glyph->bitmap_top - int(((FT_Face)bitmap_font->face)->glyph->bitmap.rows) - (float)params.outline_size_64 / 64,
            (float)((FT_Face)bitmap_font->face)->glyph->advance.x / 64,
            (float)((FT_Face)bitmap_font->face)->glyph->advance.y / 64
    )).first;

    FT_Stroker_Done(stroker);

    return &iter->second;
}

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
    return characters_sets.emplace(params, CharacterSet(this, params)).first->second;
}

BitmapFont::BitmapFont(const std::vector<unsigned char> &ttf) : data(ttf) {
    FT_New_Memory_Face((FT_Library)GetFreeTypeLibrary(), &data[0], data.size(), 0, (FT_Face*)&face);
}

BitmapFont::~BitmapFont() {
    FT_Done_Face((FT_Face)face);
}

void *BitmapFont::GetFreeTypeLibrary() {
    static FT_Library ft_library;
    if (!ft_library) {
        FT_Init_FreeType(&ft_library);
    }
    return ft_library;
}

intrusive_ptr<BitmapFont> BitmapFont::Create(const std::vector<unsigned char> &ttf) {
    return new BitmapFont(ttf);
}

} //namespace a2d

