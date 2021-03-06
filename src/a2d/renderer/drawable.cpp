//
// Created by selya on 05.12.2018.
//

#include <a2d/core/resources.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/renderer/material.hpp>
#include <a2d/renderer/shader.hpp>
#include <a2d/renderer/drawable.hpp>
#include <a2d/renderer/texture/texture_region.hpp>

namespace a2d {

Drawable::Drawable() : material(Material::Create(Resources::Get<Shader>("default"))) {};
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

bool Drawable::IsLess(const Drawable &other) const {
    if (material && other.material && material->GetHash() != other.material->GetHash())
        return material->GetHash() < other.material->GetHash();
    return this < &other;
}

const intrusive_ptr<Material> Drawable::GetMaterial() const {
    return material;
}

const intrusive_ptr<TextureRegion> Drawable::GetTextureRegion() const {
    return texture_region;
}

void Drawable::SetMaterial(const intrusive_ptr<Material> &material) {
    intrusive_ptr<Object2D> o = GetObject2D();
    if (o) {
        this->material = material;
        if (texture_region && texture_region->GetTexture() && material)
            material->SetTexture("main_texture", texture_region->GetTexture());
    } else this->material = material;
}

void Drawable::SetTextureRegion(const intrusive_ptr<TextureRegion> &texture_region) {
    intrusive_ptr<Object2D> o = GetObject2D();
    if (o) {
        this->texture_region = texture_region;
        if (texture_region && texture_region->GetTexture() && material)
            material->SetTexture("main_texture", texture_region->GetTexture());
    } else this->texture_region = texture_region;
}

void Drawable::Draw(SpriteBatch &sprite_batch, const Vector4f &color) {}

void Drawable::Initialize() {
    if (!object_2d->drawable)
        object_2d->drawable = this;
    else
        LOG_TRACE("Added second drawable");
}

void Drawable::OnDestroy() {
    if (object_2d->drawable == this)
        object_2d->drawable = nullptr;
}

} //namespace a2d
