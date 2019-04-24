//
// Created by selya on 04.11.2018.
//

#include <a2d/core/object2d.hpp>
#include <a2d/core/log.hpp>
#include <a2d/physics/rigidbody.hpp>
#include <a2d/physics/physics_collider.hpp>
#include <a2d/utils/utils.hpp>
#include "object2d.hpp"


namespace a2d {

Object2D::Object2D() :
parent(), children(), iter_in_parent(), components(),
transform_matrix(), local_position(), local_scale(1), local_rotation(0),
layer(0), color(1), active(true), active_transitive(true), tag(""), drawable() {}

bool Object2D::compare_objects::operator()(const intrusive_ptr<Object2D> &lhs, const intrusive_ptr<Object2D> &rhs) const {
    if (lhs->layer != rhs->layer) return lhs->layer < rhs->layer;
    if (lhs->drawable && rhs->drawable) {
        return lhs->drawable->IsLess(*rhs->drawable);
    }
    return lhs < rhs;
}


Object2D::~Object2D() {}


//
// Getters
//


intrusive_ptr<Object2D> Object2D::GetParent() const {
    return parent;
}

const std::list<intrusive_ptr<Object2D>> &Object2D::GetChildren() const {
    return children;
}

const Matrix4f &Object2D::GetTransformMatrix() const {
    return transform_matrix;
}

const Vector2f &Object2D::GetLocalPosition() const {
    return local_position;
}

const Vector2f &Object2D::GetLocalScale() const {
    return local_scale;
}

float Object2D::GetLocalRotation() const {
    return local_rotation;
}

Vector2f Object2D::GetPosition() const {
    Vector3f translation = transform_matrix.GetTranslation();
    return Vector2f(translation.x, translation.y);
}

float Object2D::GetRotation() const {
    return transform_matrix.GetRotationZ();
}

Vector2f Object2D::GetRelativePosition(const intrusive_ptr<Object2D> &origin) const {
    if (!origin) {
        LOG_TRACE("Origin object is null");
        return Vector2f();
    }
    Vector3f t = (Matrix4f(origin->transform_matrix).Inverse() * transform_matrix).GetTranslation();
    return Vector2f(t.x, t.y);
}

float Object2D::GetRelativeRotation(const intrusive_ptr<Object2D> &origin) const {
    if (!origin) {
        LOG_TRACE("Origin object is null");
        return 0;
    }
    return (Matrix4f(origin->transform_matrix).Inverse() * transform_matrix).GetRotationZ();
}

Vector2f Object2D::WorldToLocalPoint(const Vector2f &world_point) {
    auto v = Matrix4f(transform_matrix).Inverse().Transform(world_point.x, world_point.y, 0, 1);
    return Vector2f(v.x, v.y);
}

Vector2f Object2D::LocalToWorldPoint(const Vector2f &local_point) {
    auto v = transform_matrix.Transform(local_point.x, local_point.y, 0, 1);
    return Vector2f(v.x, v.y);
}


int Object2D::GetLayer() const {
    return layer;
}

const Vector4f &Object2D::GetColor() const {
    return color;
}


bool Object2D::IsActive() const {
    return active;
}

bool Object2D::IsActiveTransitive() const {
    return active_transitive;
}

const std::string &Object2D::GetTag() const {
    return tag;
}

intrusive_ptr<Drawable> Object2D::GetDrawable() const {
    return drawable;
}


//
// Setters
//


void Object2D::SetLocalPosition(float x, float y) {
    local_position.Set(x, y);
    OnTransform(this);
}

void Object2D::SetLocalPosition(const Vector2f &position) {
    SetLocalPosition(position.x, position.y);
}

void Object2D::SetLocalScale(float x, float y) {
    local_scale.Set(x, y);
    OnTransform(this, true, true);
}

void Object2D::SetLocalScale(const Vector2f &scale) {
    SetLocalScale(scale.x, scale.y);
}

void Object2D::SetLocalRotation(float rotation) {
    this->local_rotation = rotation;
    OnTransform(this);
}

void Object2D::SetPosition(float x, float y) {
    if (parent) {
        local_position = parent->GetPosition();
        local_position.x = x - local_position.x;
        local_position.y = y - local_position.y;
    } else {
        local_position.Set(x, y);
    }
    OnTransform(this);
}

void Object2D::SetPosition(const Vector2f &position) {
    SetPosition(position.x, position.y);
}

void Object2D::SetRotation(float rotation) {
    if (parent) {
        local_rotation = rotation - parent->GetRotation();
    } else {
        local_rotation = rotation;
    }
    OnTransform(this);
}

void Object2D::SetLayer(int layer) {
    this->layer = layer;
}

void Object2D::SetColor(float r, float g, float b, float a) {
    color.Set(r, g, b, a);
}

void Object2D::SetColor(const a2d::Vector4f &color) {
    SetColor(color.x, color.y, color.z, color.w);
}


void Object2D::SetActive(bool active) {
    this->active = active;
    bool old_active_transitive = this->active_transitive;
    this->active_transitive = (parent ? parent->active_transitive : true) && active;
    if (old_active_transitive != this->active_transitive) {
        for (auto &o : components) {
            for (auto &c : o.second) {
                c->SetActive(c->IsActive());
            }
        }
        for (auto &c : children) {
            c->SetActive(c->IsActive());
        }
    }
}

void Object2D::SetTag(const std::string &tag) {
    this->tag = tag;
}


//
// Manage
//


void Object2D::Attach(const intrusive_ptr<Object2D> &parent) {
    if (!parent) {
        LOG_TRACE("Parent is null");
        return;
    }
    if (parent != this->parent) {
        intrusive_ptr<Object2D> o = this;
        if (o->parent) {
            OnDetach();
            o->parent->children.erase(iter_in_parent);
        }
        o->parent = parent;
        iter_in_parent = o->parent->children.insert(o->parent->children.end(), this);
        SetActive(IsActive());
        OnTransform(this, true, true);
        OnAttach();
    }
}

void Object2D::Destroy() {
    for (auto &c : children) c->Destroy();
    DestroyAllComponents();
    intrusive_ptr<Object2D> o = this;
    Engine::AddCommand([o]() {
        if (!o->parent) return;
        o->parent->children.erase(o->iter_in_parent);
        o->parent = nullptr;
    });
}

void Object2D::DestroyAllComponents() {
    for (auto &c : components) {
        for (const intrusive_ptr<Component> &component : c.second) {
            component->Destroy();
        }
    }
}


//
// Draw
//


void Object2D::Draw(SpriteBatch &sprite_batch, const Vector4f &parent_color) {
    auto local_color = parent_color * color;
    if (drawable && drawable->IsActiveTransitive()) drawable->Draw(sprite_batch, local_color);
    // TODO will timsort sort faster?
    children.sort(compare_objects());
    for (const intrusive_ptr<Object2D> &c : children) {
        if (c->IsActiveTransitive()) c->Draw(sprite_batch, local_color);
    }
}


//
// Events
//


void Object2D::OnAttach() {
    for (auto &c : components) {
        for (const intrusive_ptr<Component> &component : c.second) {
            if (component->initialized) component->OnAttach();
        }
    }
    for (auto &c : children) {
        c->OnAttach();
    }
}

void Object2D::OnDetach() {
    for (auto &c : components) {
        for (const intrusive_ptr<Component> &component : c.second) {
            if (component->initialized) component->OnDetach();
        }
    }
    for (auto &c : children) {
        c->OnDetach();
    }
}

void Object2D::OnTransform(const intrusive_ptr<Object2D> &object, bool apply_local, bool scaling) {
    if (apply_local) {
        transform_matrix.Identity();
        transform_matrix.Translate(local_position.x, local_position.y, 0.0f);
        transform_matrix.Rotate(local_rotation, 0.0f, 0.0f, 1.0f);
        transform_matrix.Scale(local_scale.x, local_scale.y, 1.0f);
    }

    if (parent) {
        transform_matrix = parent->transform_matrix * transform_matrix;
    }

    for (auto &c : children) {
        c->OnTransform(object);
    }

    auto rigidbody = GetComponent<Rigidbody>();
    if (rigidbody) {
        rigidbody->Transform(GetPosition(), GetRotation());
    }

    // TODO optimize
    auto collider = GetComponent<PhysicsCollider>(true);
    if (collider) {
        // Reattach always on scaling
        if (scaling) collider->Reattach();
        else {
            auto o = this;
            if (collider->GetRigidbody()) {
                auto r = collider->GetRigidbody()->GetObject2D().get();
                while (o) {
                    if (o == r) break;
                    if (o == object.get()) {
                        collider->Reattach();
                        break;
                    }
                    o = o->GetParent().get();
                }
            }
        }
    }
}


intrusive_ptr<Object2D> Object2D::Create() {
    intrusive_ptr<Object2D> o = new Object2D;
    if (Engine::GetRoot()) o->Attach(Engine::GetRoot());
    return o;
}

} //namespace a2d
