//
// Created by selya on 04.11.2018.
//

#include <a2d/core/object2d.hpp>

namespace a2d {

bool Object2D::compare_objects::operator()(const pObject2D &lhs, const pObject2D &rhs) const {
    ASSERT_MAIN_THREAD
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
    ASSERT_MAIN_THREAD
}

Object2D::~Object2D() {
    ASSERT_MAIN_THREAD
}

int Object2D::GetLayer() {
    ASSERT_MAIN_THREAD
    return layer;
}

pObject2D Object2D::GetParent() const {
    ASSERT_MAIN_THREAD
    return parent;
}

const Matrix4f &Object2D::GetTransformMatrix() const {
    ASSERT_MAIN_THREAD
    return transform_matrix;
}

const Matrix4f &Object2D::GetTransformMatrixRecalculated(bool recursive) {
    ASSERT_MAIN_THREAD
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
    ASSERT_MAIN_THREAD
    pObject2D o = this; // prevent this object destruction
    if (parent) parent->children.erase(o);
    this->layer = layer;
    if (parent) parent->children.emplace(o);
}

void Object2D::Attach(const pObject2D &parent) {
    ASSERT_MAIN_THREAD
    ASSERT(parent != nullptr)
    if (parent != this->parent) {
        if (this->parent) this->parent->children.erase(this);
        this->parent = parent;
        parent->children.emplace(this);
    }
}

void Object2D::Destroy() {
    ASSERT_MAIN_THREAD
    for (auto &c : children) c->Destroy();
    DestroyAllComponents();
    Engine::AddCommand([this]() {
        if (parent) parent->children.erase(this);
    });
}

void Object2D::Draw(const a2d::Matrix4f &parent_transform, SpriteBatch &sprite_batch) {
    ASSERT_MAIN_THREAD
    GetTransformMatrixRecalculated(false);

    for (auto &drawable : drawables) {
        drawable->Draw(sprite_batch);
    }
    for (const pObject2D &c : children) {
        c->Draw(transform_matrix, sprite_batch);
    }
}

void Object2D::DestroyAllComponents() {
    ASSERT_MAIN_THREAD
    for (auto &c : components) {
        for (const pComponent &component : c.second) {
            component->Destroy();
        }
    }
}

Vector2f Object2D::WorldToLocal(const Vector2f &world_point) {
    ASSERT_MAIN_THREAD
    auto v = Matrix4f(GetTransformMatrixRecalculated()).Inverse().Transform(world_point.x, world_point.y, 0, 1);
    return Vector2f(v.x, v.y);
}

Vector2f Object2D::LocalToWorld(const Vector2f &local_point) {
    ASSERT_MAIN_THREAD
    auto v = GetTransformMatrixRecalculated().Transform(local_point.x, local_point.y, 0, 1);
    return Vector2f(v.x, v.y);
}

pObject2D Object2D::Create() {
    ASSERT_MAIN_THREAD
    pObject2D o = new Object2D;
    if (Engine::GetRoot()) o->Attach(Engine::GetRoot());
    return o;
}

} //namespace a2d
