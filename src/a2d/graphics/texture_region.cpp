//
// Created by selya on 13.11.2018.
//

#include <a2d/graphics/texture_region.hpp>
#include <a2d/core/engine.hpp>

namespace a2d {

TextureRegion::TextureRegion() : TextureRegion(nullptr, 0, 0, 0, 0) {
    ASSERT_MAIN_THREAD
}

TextureRegion::TextureRegion(
        pTexture texture,
        Texture::Filtering filtering,
        Texture::Wrapping wrapping
) : TextureRegion(texture, 0, 0, texture->GetWidth(), texture->GetHeight(), filtering, wrapping) {
    ASSERT_MAIN_THREAD
}

TextureRegion::TextureRegion(
        pTexture texture,
        int x, int y, int width, int height,
        Texture::Filtering filtering,
        Texture::Wrapping wrapping
) : texture(texture),
    offset(x, y), size(width, height),
    uv_lb(0), uv_rt(0),
    filtering(filtering), wrapping(wrapping), flags(0) {
    ASSERT_MAIN_THREAD
    RecalculateUV();
}

TextureRegion::~TextureRegion() {
    ASSERT_MAIN_THREAD
};

void TextureRegion::SetTexture(const pTexture &texture) {
    ASSERT_MAIN_THREAD
    this->texture = texture;
}

void TextureRegion::SetX(int x) {
    ASSERT_MAIN_THREAD
    offset.x = x;
}

void TextureRegion::SetY(int y) {
    ASSERT_MAIN_THREAD
    offset.y = y;
}

void TextureRegion::SetWidth(int width) {
    ASSERT_MAIN_THREAD
    size.x = width;
}

void TextureRegion::SetHeight(int height) {
    ASSERT_MAIN_THREAD
    size.y = height;
}

void TextureRegion::SetFiltering(Texture::Filtering filtering) {
    ASSERT_MAIN_THREAD
    this->filtering = filtering;
}

void TextureRegion::SetWrapping(Texture::Wrapping wrapping) {
    ASSERT_MAIN_THREAD
    this->wrapping = wrapping;
}

void TextureRegion::SetFlippedHorizontally(bool flip) {
    ASSERT_MAIN_THREAD
    if (flip) flags |= (unsigned char)1;
    else flags &= ~(unsigned char)1;
}

void TextureRegion::SetFlippedVertically(bool flip) {
    ASSERT_MAIN_THREAD
    if (flip) flags |= (unsigned char)2;
    else flags &= ~(unsigned char)2;
}

pTexture TextureRegion::GetTexture() const {
    ASSERT_MAIN_THREAD
    return texture;
}

int TextureRegion::GetX() const {
    ASSERT_MAIN_THREAD
    return offset.x;
}

int TextureRegion::GetY() const {
    ASSERT_MAIN_THREAD
    return offset.y;
}

int TextureRegion::GetWidth() const {
    ASSERT_MAIN_THREAD
    return size.x;
}

int TextureRegion::GetHeight() const {
    ASSERT_MAIN_THREAD
    return size.y;
}

float TextureRegion::GetRatio() const {
    ASSERT_MAIN_THREAD
    return ratio;
}

Texture::Filtering TextureRegion::GetFiltering() const {
    ASSERT_MAIN_THREAD
    return filtering;
}

Texture::Wrapping TextureRegion::GetWrapping() const {
    ASSERT_MAIN_THREAD
    return wrapping;
}

bool TextureRegion::IsFlippedHorizontally() const {
    ASSERT_MAIN_THREAD
    return bool(flags & (unsigned char)1);
}

bool TextureRegion::IsFlippedVertically() const {
    ASSERT_MAIN_THREAD
    return bool(flags & 2);
}

void TextureRegion::Bind(unsigned int texture_unit) {
    ASSERT_MAIN_THREAD
    if (!texture) return;
    RecalculateUV();
    texture->Bind(texture_unit, filtering, wrapping);
}

void TextureRegion::RecalculateUV() {
    ASSERT_MAIN_THREAD
    if (!texture) return;
    // TODO optimize

    uv_lb = Vector2f(offset) / Vector2f(float(texture->GetWidth()), float(texture->GetHeight()));
    uv_rt = Vector2f(float(offset.x + size.x), float(offset.y + size.y)) /
            Vector2f(float(texture->GetWidth()), float(texture->GetHeight()));

    if (IsFlippedHorizontally()) {
        float x = uv_lb.x;
        uv_lb.x = uv_rt.x;
        uv_rt.x = x;
    }

    if (IsFlippedVertically()) {
        float y = uv_lb.y;
        uv_lb.y = uv_rt.y;
        uv_rt.y = y;
    }

    ratio = std::abs(float(size.x) / size.y);
}

pTextureRegion TextureRegion::Create() {
    ASSERT_MAIN_THREAD
    return new TextureRegion;
}

pTextureRegion TextureRegion::Create(pTexture texture, Texture::Filtering filtering, Texture::Wrapping wrapping) {
    ASSERT_MAIN_THREAD
    return new TextureRegion(texture, filtering, wrapping);
}

pTextureRegion
TextureRegion::Create(pTexture texture, int x, int y, int width, int height,
                      Texture::Filtering filtering, Texture::Wrapping wrapping) {
    ASSERT_MAIN_THREAD
    return new TextureRegion(texture, x, y, width, height, filtering, wrapping);
}

} //namespace a2d
