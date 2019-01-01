//
// Created by selya on 05.12.2018.
//

#include <a2d/core/drawable.h>

namespace a2d {

float Drawable::GetWidth() const {
    return size.x;
}

float Drawable::GetHeight() const {
    return size.y;
}

Vector2f Drawable::GetSize() const {
    return Vector2f(GetWidth(), GetHeight());
}

Vector2f Drawable::GetOrigin() const {
    return origin;
}

void Drawable::SetWidth(float width) {
    size.x = width;
}

void Drawable::SetHeight(float height) {
    size.y = height;
}

void Drawable::SetSize(float x, float y) {
    SetWidth(x);
    SetHeight(y);
}

void Drawable::SetSize(const Vector2f &size) {
    SetWidth(size.x);
    SetHeight(size.y);
}

void Drawable::SetOrigin(float x, float y) {
    origin.Set(x, y);
}

void Drawable::SetOrigin(const Vector2f &origin) {
    this->origin.Set(origin);
}

bool Drawable::operator<(const Drawable &other) const {
    const Shader *s1, *s2;
    s1 = GetShaderForSortOrNull();
    s2 = other.GetShaderForSortOrNull();
    if (s1 != s2) return s1 < s2;
    const TextureRegion *t1, *t2;
    t1 = GetTextureRegionForSortOrNull();
    t2 = other.GetTextureRegionForSortOrNull();
    if (t1 && t2 && t1->GetTexture() != t2->GetTexture()) return t1->GetTexture() < t2->GetTexture();
    // TODO implement wrapping and filtering sort
    return this < &other;
}

const Shader *Drawable::GetShaderForSortOrNull() const {
    return nullptr;
}

const TextureRegion *Drawable::GetTextureRegionForSortOrNull() const {
    return nullptr;
}

void Drawable::Draw(SpriteBatch &sprite_batch) {}

Drawable::~Drawable() = default;


} //namespace a2d
