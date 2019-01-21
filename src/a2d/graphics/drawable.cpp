//
// Created by selya on 05.12.2018.
//

#include <a2d/graphics/drawable.hpp>
#include <a2d/core/resources.hpp>
#include <a2d/core/object2d.hpp>

namespace a2d {

Drawable::Drawable() : shader(Resources::Get<Shader>("default")){};
Drawable::~Drawable() = default;

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
    if (shader != other.shader) return shader < other.shader;
    const TextureRegion *t1, *t2;
    t1 = texture_region.get();
    t2 = other.texture_region.get();
    if (t1 && t2) {
        if (t1->GetTexture() != t2->GetTexture()) {
            return t1->GetTexture() < t2->GetTexture();
        }
        if (t1->GetFiltering() != t2->GetFiltering()) {
            return t1->GetFiltering() < t2->GetFiltering();
        }
        if (t1->GetWrapping() != t2->GetWrapping()) {
            return t1->GetWrapping() < t2->GetWrapping();
        }
    }
    return this < &other;
}

const pShader Drawable::GetShader() const {
    return shader;
}

const pTextureRegion Drawable::GetTextureRegion() const {
    return texture_region;
}

void Drawable::SetShader(const pShader &shader) {
    pObject2D o = GetObject2D();
    if (o) {
        if (o->parent) o->parent->children.erase(o);
        this->shader = shader;
        if (o->parent) o->parent->children.emplace(o);
    } else this->shader = shader;
}

void Drawable::SetTextureRegion(const pTextureRegion &texture_region) {
    pObject2D o = GetObject2D();
    if (o) {
        if (o->parent) o->parent->children.erase(o);
        this->texture_region = texture_region;
        if (o->parent) o->parent->children.emplace(o);
    } else this->texture_region = texture_region;
}

void Drawable::Draw(SpriteBatch &sprite_batch) {}

void Drawable::Initialize() {
    pObject2D o = object_2d;
    if (object_2d->parent) object_2d->parent->children.erase(o);
    object_2d->drawables.emplace(this);
    if (object_2d->parent) object_2d->parent->children.emplace(o);
}

void Drawable::OnDestroy() {
    pObject2D o = object_2d;
    if (object_2d->parent) object_2d->parent->children.erase(o);
    object_2d->drawables.erase(this);
    if (object_2d->parent) object_2d->parent->children.emplace(o);
}

} //namespace a2d
