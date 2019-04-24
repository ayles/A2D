//
// Created by selya on 13.11.2018.
//

#include <a2d/renderer/texture/texture_region.hpp>
#include <a2d/renderer/texture/texture.hpp>
#include <a2d/core/engine.hpp>


namespace a2d {

TextureRegion::TextureRegion() : TextureRegion(nullptr, 0, 0, 0, 0) {}

TextureRegion::TextureRegion(intrusive_ptr<Texture> texture) :
TextureRegion(texture, 0, 0, texture->GetWidth(), texture->GetHeight()) {}

TextureRegion::TextureRegion(intrusive_ptr<Texture> texture, int x, int y, int width, int height) :
texture(texture), offset(x, y), size(width, height), uv_lower(0), uv_upper(0), flags(0) {
    RecalculateUV();
}

TextureRegion::~TextureRegion() {}

void TextureRegion::SetTexture(const intrusive_ptr<Texture> &texture) {
    this->texture = texture;
    RecalculateUV();
}

void TextureRegion::SetX(int x) {
    offset.x = x;
    RecalculateUV();
}

void TextureRegion::SetY(int y) {
    offset.y = y;
    RecalculateUV();
}

void TextureRegion::SetWidth(int width) {
    size.x = width;
    RecalculateUV();
}

void TextureRegion::SetHeight(int height) {
    size.y = height;
    RecalculateUV();
}

void TextureRegion::SetFlippedHorizontally(bool flip) {
    if (flip) flags |= (unsigned char)1;
    else flags &= ~(unsigned char)1;
    RecalculateUV();
}

void TextureRegion::SetFlippedVertically(bool flip) {
    if (flip) flags |= (unsigned char)2;
    else flags &= ~(unsigned char)2;
    RecalculateUV();
}

intrusive_ptr<Texture> TextureRegion::GetTexture() const {
    return texture;
}

int TextureRegion::GetX() const {
    return offset.x;
}

int TextureRegion::GetY() const {
    return offset.y;
}

int TextureRegion::GetWidth() const {
    return size.x;
}

int TextureRegion::GetHeight() const {
    return size.y;
}

float TextureRegion::GetRatio() const {
    return ratio;
}

bool TextureRegion::IsFlippedHorizontally() const {
    return bool(flags & (unsigned char)1);
}

bool TextureRegion::IsFlippedVertically() const {
    return bool(flags & 2);
}

void TextureRegion::RecalculateUV() {
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

intrusive_ptr<TextureRegion> TextureRegion::Create() {
    return new TextureRegion;
}

intrusive_ptr<TextureRegion> TextureRegion::Create(intrusive_ptr<Texture> texture) {
    return new TextureRegion(texture);
}

intrusive_ptr<TextureRegion>
TextureRegion::Create(intrusive_ptr<Texture> texture, int x, int y, int width, int height) {
    return new TextureRegion(texture, x, y, width, height);
}

const Vector2f &TextureRegion::GetUVLower() const {
    return uv_lower;
}

const Vector2f &TextureRegion::GetUVUpper() const {
    return uv_upper;
}

} //namespace a2d
