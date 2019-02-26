//
// Created by selya on 04.11.2018.
//

#include <a2d/core/object2d.hpp>
#include <a2d/core/log.hpp>
#include <a2d/physics/rigidbody.hpp>
#include <a2d/physics/physics_collider.hpp>
#include <a2d/utils/utils.hpp>


namespace a2d {

bool Object2D::compare_objects::operator()(const pObject2D &lhs, const pObject2D &rhs) const {
    ASSERT_MAIN_THREAD
    if (lhs->layer != rhs->layer) return lhs->layer < rhs->layer;
    pDrawable left = lhs->drawables.empty() ? nullptr : *(lhs->drawables.begin());
    pDrawable right = rhs->drawables.empty() ? nullptr : *(rhs->drawables.begin());
    if (left && right) {
        return left->IsLess(*right);
    }
    return lhs < rhs;
}

Object2D::Object2D() : parent(nullptr), layer(0),  local_position(), local_scale(1), local_rotation(0),
active(true), active_transitive(true)
{
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

void Object2D::SetLayer(int layer) {
    ASSERT_MAIN_THREAD
    //pObject2D o = this; // prevent this object destruction
    //if (parent) parent->children.erase(o);
    this->layer = layer;
    //if (parent) parent->children.emplace(o);
}

void Object2D::Attach(const pObject2D &parent) {
    ASSERT_MAIN_THREAD
    if (!parent) {
        LOG_TRACE("Parent is null");
        return;
    }
    if (parent != this->parent) {
        pObject2D o = this;
        if (o->parent) {
            OnDetach();
            o->parent->children.erase(iter_in_parent);
            //this->parent->children.erase(this);
        }
        o->parent = parent;
        iter_in_parent = o->parent->children.insert(o->parent->children.end(), this);
        //parent->children.emplace(this);
        // Retrigger activation recursion
        SetActive(IsActive());
        OnTransform(this, true, true);
        OnAttach();
    }
}

void Object2D::Destroy() {
    ASSERT_MAIN_THREAD
    for (auto &c : children) c->Destroy();
    DestroyAllComponents();
    pObject2D o = this;
    Engine::AddCommand([o]() {
        if (!o->parent) return;
        o->parent->children.erase(o->iter_in_parent);
        o->parent = nullptr;
    });
}

void Object2D::Draw(SpriteBatch &sprite_batch) {
    ASSERT_MAIN_THREAD
    for (auto &drawable : drawables) {
        if (!drawable->IsActiveTransitive()) continue;
        drawable->Draw(sprite_batch);
    }
    // TODO will timsort sort faster?
    children.sort(compare_objects());
    for (const pObject2D &c : children) {
        if (!c->IsActiveTransitive()) continue;
        c->Draw(sprite_batch);
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

Vector2f Object2D::WorldToLocalPoint(const Vector2f &world_point) {
    ASSERT_MAIN_THREAD
    auto v = Matrix4f(transform_matrix).Inverse().Transform(world_point.x, world_point.y, 0, 1);
    return Vector2f(v.x, v.y);
}

Vector2f Object2D::LocalToWorldPoint(const Vector2f &local_point) {
    ASSERT_MAIN_THREAD
    auto v = transform_matrix.Transform(local_point.x, local_point.y, 0, 1);
    return Vector2f(v.x, v.y);
}

pObject2D Object2D::Create() {
    ASSERT_MAIN_THREAD
    pObject2D o = new Object2D;
    if (Engine::GetRoot()) o->Attach(Engine::GetRoot());
    return o;
}

void Object2D::OnAttach() {
    ASSERT_MAIN_THREAD
    for (auto &c : components) {
        for (const pComponent &component : c.second) {
            if (component->initialized) component->OnAttach();
        }
    }

    for (auto &c : children) {
        c->OnAttach();
    }
}

void Object2D::OnDetach() {
    ASSERT_MAIN_THREAD
    for (auto &c : components) {
        for (const pComponent &component : c.second) {
            if (component->initialized) component->OnDetach();
        }
    }

    for (auto &c : children) {
        c->OnDetach();
    }
}

Vector2f Object2D::GetPosition() const {
    ASSERT_MAIN_THREAD
    Vector3f translation = transform_matrix.GetTranslation();
    return Vector2f(translation.x, translation.y);
}

float Object2D::GetRotation() const {
    ASSERT_MAIN_THREAD
    return transform_matrix.GetRotationZ();
}

Vector2f Object2D::GetRelativePosition(const pObject2D &origin) {
    ASSERT_MAIN_THREAD
    if (!origin) {
        LOG_TRACE("Origin object is null");
        return Vector2f();
    }
    Vector3f t = (Matrix4f(origin->transform_matrix).Inverse() * transform_matrix).GetTranslation();
    return Vector2f(t.x, t.y);
}

float Object2D::GetRelativeRotation(const pObject2D &origin) {
    ASSERT_MAIN_THREAD
    if (!origin) {
        LOG_TRACE("Origin object is null");
        return 0;
    }
    return (Matrix4f(origin->transform_matrix).Inverse() * transform_matrix).GetRotationZ();
}

const Vector2f &Object2D::GetLocalPosition() const {
    ASSERT_MAIN_THREAD
    return local_position;
}

const Vector2f &Object2D::GetLocalScale() const {
    ASSERT_MAIN_THREAD
    return local_scale;
}

float Object2D::GetLocalRotation() const {
    ASSERT_MAIN_THREAD
    return local_rotation;
}

bool Object2D::IsActive() const {
    return active;
}

bool Object2D::IsActiveTransitive() const {
    return active_transitive;
}

void Object2D::SetPosition(float x, float y) {
    ASSERT_MAIN_THREAD
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
    ASSERT_MAIN_THREAD
    SetPosition(position.x, position.y);
}

void Object2D::SetRotation(float rotation) {
    ASSERT_MAIN_THREAD
    if (parent) {
        local_rotation = rotation - parent->GetRotation();
    } else {
        local_rotation = rotation;
    }
    OnTransform(this);
}

void Object2D::SetLocalPosition(float x, float y) {
    ASSERT_MAIN_THREAD
    local_position.Set(x, y);
    OnTransform(this);
}

void Object2D::SetLocalPosition(const Vector2f &position) {
    ASSERT_MAIN_THREAD
    SetLocalPosition(position.x, position.y);
}

void Object2D::SetLocalScale(float x, float y) {
    ASSERT_MAIN_THREAD
    local_scale.Set(x, y);
    OnTransform(this, true, true);
}

void Object2D::SetLocalScale(const Vector2f &scale) {
    ASSERT_MAIN_THREAD
    SetLocalScale(scale.x, scale.y);
}

void Object2D::SetLocalRotation(float rotation) {
    ASSERT_MAIN_THREAD
    this->local_rotation = rotation;
    OnTransform(this);
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

void Object2D::OnTransform(const pObject2D &object, bool apply_local, bool scaling) {
    ASSERT_MAIN_THREAD
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

} //namespace a2d
