//
// Created by selya on 04.11.2018.
//

#include <a2d/core/object2d.hpp>
#include "object2d.hpp"


namespace a2d {

bool Object2D::compare_objects::operator()(const pObject2D &lhs, const pObject2D &rhs) const {
    if (lhs->layer != rhs->layer) return lhs->layer < rhs->layer;
    pDrawable left = lhs->drawables.empty() ? nullptr : *(lhs->drawables.begin());
    pDrawable right = rhs->drawables.empty() ? nullptr : *(rhs->drawables.begin());
    if (left != right) {
        if (left && right) {
            return *(left) < *(right);
        } else {
            return left < right;
        }
    }
    return lhs < rhs;
}

Object2D::Object2D() : parent(nullptr), layer(0),  position(), scale(1), rotation(), components() {
    if (Engine::GetRoot()) Engine::GetRoot()->AddChild(this);
}
Object2D::~Object2D() {}

int Object2D::GetLayer() {
    return layer;
}

pObject2D Object2D::GetParent() const {
    return parent;
}

const Matrix4f &Object2D::GetTransformMatrix() const {
    return transform_matrix;
}

const Matrix4f &Object2D::GetTransformMatrixRecalculated(bool recursive) {
    transform_matrix.Identity();
    transform_matrix.Translate(position.x, position.y, 0.0f);
    transform_matrix.Rotate(rotation, 0.0f, 0.0f, 1.0f);
    transform_matrix.Scale(scale.x, scale.y, 1.0f);

    if (!parent) return transform_matrix;
    return (transform_matrix =
            (recursive ? parent->GetTransformMatrixRecalculated() : parent->GetTransformMatrix()) *
            transform_matrix);
}

void Object2D::SetLayer(int layer) {
    pObject2D o = this; // prevent this object destruction
    if (parent) parent->children.erase(o);
    this->layer = layer;
    if (parent) parent->children.emplace(o);
}

pObject2D Object2D::AddChild(const pObject2D &child) {
    if (child->parent != this) {
        if (child->parent) child->parent->children.erase(child);
        child->parent = this;
        children.emplace(child);
    }
    return child;
}

void Object2D::Destroy() {
    for (auto &c : children) c->Destroy();
    DestroyAllComponents();
    Engine::AddCommand([this]() {
        if (parent) parent->children.erase(this);
    });
}

void Object2D::Draw(const a2d::Matrix4f &parent_transform, SpriteBatch &sprite_batch) {
    GetTransformMatrixRecalculated(false);

    for (auto &drawable : drawables) {
        drawable->Draw(sprite_batch);
    }
    for (const pObject2D &c : children) {
        c->Draw(transform_matrix, sprite_batch);
    }
}

void Object2D::DestroyAllComponents() {
    for (auto &c : components) {
        for (const pComponent &component : c.second) {
            component->Destroy();
        }
    }
}

Vector2f Object2D::WorldToLocal(const Vector2f &world_point) {
    auto v = Matrix4f(GetTransformMatrixRecalculated()).Inverse().Transform(world_point.x, world_point.y, 0, 1);
    return Vector2f(v.x, v.y);
}

Vector2f Object2D::LocalToWorld(const Vector2f &local_point) {
    auto v = GetTransformMatrixRecalculated().Transform(local_point.x, local_point.y, 0, 1);
    return Vector2f(v.x, v.y);
}

} //namespace a2d
