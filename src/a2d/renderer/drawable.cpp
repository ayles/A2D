//
// Created by selya on 05.12.2018.
//

#include <a2d/renderer/drawable.hpp>
#include <a2d/core/resources.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/engine.hpp>


namespace a2d {

Drawable::Drawable() : material(Material::Create(Resources::Get<Shader>("default"))) {};
Drawable::~Drawable() = default;

float Drawable::GetWidth() const {
    ASSERT_MAIN_THREAD
    return size.x;
}

float Drawable::GetHeight() const {
    ASSERT_MAIN_THREAD
    return size.y;
}

Vector2f Drawable::GetSize() const {
    ASSERT_MAIN_THREAD
    return Vector2f(GetWidth(), GetHeight());
}

Vector2f Drawable::GetOrigin() const {
    ASSERT_MAIN_THREAD
    return origin;
}

void Drawable::SetWidth(float width) {
    ASSERT_MAIN_THREAD
    size.x = width;
}

void Drawable::SetHeight(float height) {
    ASSERT_MAIN_THREAD
    size.y = height;
}

void Drawable::SetSize(float x, float y) {
    ASSERT_MAIN_THREAD
    SetWidth(x);
    SetHeight(y);
}

void Drawable::SetSize(const Vector2f &size) {
    ASSERT_MAIN_THREAD
    SetWidth(size.x);
    SetHeight(size.y);
}

void Drawable::SetOrigin(float x, float y) {
    ASSERT_MAIN_THREAD
    origin.Set(x, y);
}

void Drawable::SetOrigin(const Vector2f &origin) {
    ASSERT_MAIN_THREAD
    this->origin.Set(origin);
}

bool Drawable::operator<(const Drawable &other) const {
    ASSERT_MAIN_THREAD
    if (material && other.material && material->GetHash() != other.material->GetHash())
        return material->GetHash() < other.material->GetHash();
    // TODO rework comparison
    if (material != other.material) return material < other.material;
    return this < &other;
}

const pMaterial Drawable::GetMaterial() const {
    ASSERT_MAIN_THREAD
    return material;
}

const pTextureRegion Drawable::GetTextureRegion() const {
    ASSERT_MAIN_THREAD
    return texture_region;
}

void Drawable::SetMaterial(const pMaterial &material) {
    ASSERT_MAIN_THREAD
    pObject2D o = GetObject2D();
    if (o) {
        if (o->parent) o->parent->children.erase(o);
        this->material = material;
        if (texture_region && texture_region->GetTexture() && material)
            material->SetTexture("main_texture", texture_region->GetTexture());
        if (o->parent) o->parent->children.emplace(o);
    } else this->material = material;
}

void Drawable::SetTextureRegion(const pTextureRegion &texture_region) {
    ASSERT_MAIN_THREAD
    pObject2D o = GetObject2D();
    if (o) {
        if (o->parent) o->parent->children.erase(o);
        this->texture_region = texture_region;
        if (texture_region && texture_region->GetTexture() && material)
            material->SetTexture("main_texture", texture_region->GetTexture());
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
