//
// Created by selya on 13.11.2018.
//

#include <a2d/renderer/texture/texture_region.hpp>
#include <a2d/core/engine.hpp>
#include "texture_region.hpp"


namespace a2d {

TextureRegion::TextureRegion() : TextureRegion(nullptr, 0, 0, 0, 0) {
    ASSERT_MAIN_THREAD
}

TextureRegion::TextureRegion(pTexture texture) :
TextureRegion(texture, 0, 0, texture->GetWidth(), texture->GetHeight()) {
    ASSERT_MAIN_THREAD
}

TextureRegion::TextureRegion(pTexture texture, int x, int y, int width, int height) :
texture(texture), offset(x, y), size(width, height), uv_lower(0), uv_upper(0), flags(0) {
    ASSERT_MAIN_THREAD
    RecalculateUV();
}

TextureRegion::~TextureRegion() {
    ASSERT_MAIN_THREAD
};

void TextureRegion::SetTexture(const pTexture &texture) {
    ASSERT_MAIN_THREAD
    this->texture = texture;
    RecalculateUV();
}

void TextureRegion::SetX(int x) {
    ASSERT_MAIN_THREAD
    offset.x = x;
    RecalculateUV();
}

void TextureRegion::SetY(int y) {
    ASSERT_MAIN_THREAD
    offset.y = y;
    RecalculateUV();
}

void TextureRegion::SetWidth(int width) {
    ASSERT_MAIN_THREAD
    size.x = width;
    RecalculateUV();
}

void TextureRegion::SetHeight(int height) {
    ASSERT_MAIN_THREAD
    size.y = height;
    RecalculateUV();
}

void TextureRegion::SetFlippedHorizontally(bool flip) {
    ASSERT_MAIN_THREAD
    if (flip) flags |= (unsigned char)1;
    else flags &= ~(unsigned char)1;
    RecalculateUV();
}

void TextureRegion::SetFlippedVertically(bool flip) {
    ASSERT_MAIN_THREAD
    if (flip) flags |= (unsigned char)2;
    else flags &= ~(unsigned char)2;
    RecalculateUV();
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

bool TextureRegion::IsFlippedHorizontally() const {
    ASSERT_MAIN_THREAD
    return bool(flags & (unsigned char)1);
}

bool TextureRegion::IsFlippedVertically() const {
    ASSERT_MAIN_THREAD
    return bool(flags & 2);
}

void TextureRegion::RecalculateUV() {
    ASSERT_MAIN_THREAD
    if (!texture) return;
    uv_lower = Vector2f(offset) / Vector2f(float(texture->GetWidth()), float(texture->GetHeight()));
    uv_upper = Vector2f(offset + size) / Vector2f(float(texture->GetWidth()), float(texture->GetHeight()));

    if (IsFlippedHorizontally()) {
        float temp = uv_lower.x;
        uv_lower.x = uv_upper.x;
        uv_upper.x = temp;
    }

    if (IsFlippedVertically()) {
        float temp = uv_lower.y;
        uv_lower.y = uv_upper.y;
        uv_upper.y = temp;
    }

    ratio = std::abs(float(size.x) / size.y);
}

pTextureRegion TextureRegion::Create() {
    ASSERT_MAIN_THREAD
    return new TextureRegion;
}

pTextureRegion TextureRegion::Create(pTexture texture) {
    ASSERT_MAIN_THREAD
    return new TextureRegion(texture);
}

pTextureRegion
TextureRegion::Create(pTexture texture, int x, int y, int width, int height) {
    ASSERT_MAIN_THREAD
    return new TextureRegion(texture, x, y, width, height);
}

const Vector2f &TextureRegion::GetUVLower() const {
    return uv_lower;
}

const Vector2f &TextureRegion::GetUVUpper() const {
    return uv_upper;
}

} //namespace a2d
