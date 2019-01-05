//
// Created by selya on 13.11.2018.
//

#include <a2d/graphics/texture_region.hpp>

namespace a2d {

TextureRegion::TextureRegion() : TextureRegion(nullptr, 0, 0, 0, 0) {}

TextureRegion::TextureRegion(
        pTexture texture,
        Texture::Filtering filtering,
        Texture::Wrapping wrapping
) : TextureRegion(texture, 0, 0, texture->GetWidth(), texture->GetHeight(), filtering, wrapping) {}

TextureRegion::TextureRegion(
        pTexture texture,
        int x, int y, int width, int height,
        Texture::Filtering filtering,
        Texture::Wrapping wrapping
) : texture(texture),
    offset(x, y), size(width, height),
    uv_lb(0), uv_rt(0),
    filtering(filtering), wrapping(wrapping), flags(0) {

    RecalculateUV();
}

TextureRegion::~TextureRegion() = default;

void TextureRegion::SetTexture(const pTexture &texture) {
    this->texture = texture;
}

void TextureRegion::SetX(int x) {
    offset.x = x;
}

void TextureRegion::SetY(int y) {
    offset.y = y;
}

void TextureRegion::SetWidth(int width) {
    size.x = width;
}

void TextureRegion::SetHeight(int height) {
    size.y = height;
}

void TextureRegion::SetFiltering(Texture::Filtering filtering) {
    this->filtering = filtering;
}

void TextureRegion::SetWrapping(Texture::Wrapping wrapping) {
    this->wrapping = wrapping;
}

void TextureRegion::SetFlippedHorizontally(bool flip) {
    if (flip) flags |= (unsigned char)1;
    else flags &= ~(unsigned char)1;
}

void TextureRegion::SetFlippedVertically(bool flip) {
    if (flip) flags |= (unsigned char)2;
    else flags &= ~(unsigned char)2;
}

pTexture TextureRegion::GetTexture() const {
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

Texture::Filtering TextureRegion::GetFiltering() const {
    return filtering;
}

Texture::Wrapping TextureRegion::GetWrapping() const {
    return wrapping;
}

bool TextureRegion::IsFlippedHorizontally() const {
    return bool(flags & (unsigned char)1);
}

bool TextureRegion::IsFlippedVertically() const {
    return bool(flags & 2);
}

void TextureRegion::Bind(unsigned int texture_unit) {
    if (!texture) return;
    RecalculateUV();
    texture->Bind(texture_unit, filtering, wrapping);
}

void TextureRegion::RecalculateUV() {
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

} //namespace a2d
