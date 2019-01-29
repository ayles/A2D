//
// Created by selya on 04.11.2018.
//

#include <a2d/core/object2d.hpp>
#include <a2d/core/log.hpp>
#include <a2d/components/physics/rigidbody.hpp>
#include <a2d/components/physics/physics_collider.hpp>


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

Object2D::Object2D() : parent(nullptr), layer(0),  local_position(), local_scale(1), local_rotation(0), world_rotation(0) {
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
    pObject2D o = this; // prevent this object destruction
    if (parent) parent->children.erase(o);
    this->layer = layer;
    if (parent) parent->children.emplace(o);
}

void Object2D::Attach(const pObject2D &parent) {
    ASSERT_MAIN_THREAD
    if (!parent) {
        LOG_TRACE("Parent is null");
        return;
    }
    if (parent != this->parent) {
        if (this->parent) {
            OnDetach();
            this->parent->children.erase(this);
        }
        this->parent = parent;
        parent->children.emplace(this);
        OnAttach();
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

void Object2D::Draw(SpriteBatch &sprite_batch) {
    ASSERT_MAIN_THREAD
    for (auto &drawable : drawables) {
        drawable->Draw(sprite_batch);
    }
    for (const pObject2D &c : children) {
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
    return world_rotation;
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
    return world_rotation - origin->world_rotation;
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
        local_rotation = rotation - parent->world_rotation;
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
    OnTransform(this);
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

void Object2D::OnTransform(const pObject2D &object) {
    ASSERT_MAIN_THREAD
    transform_matrix.Identity();
    transform_matrix.Translate(local_position.x, local_position.y, 0.0f);
    transform_matrix.Rotate(local_rotation, 0.0f, 0.0f, 1.0f);
    transform_matrix.Scale(local_scale.x, local_scale.y, 1.0f);

    if (parent) {
        transform_matrix = parent->transform_matrix * transform_matrix;
        world_rotation = local_rotation + parent->world_rotation;
    } else {
        world_rotation = local_rotation;
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
    // TODO additionally check for parent-parent transformations and discard
    if (collider && object.get() != this) {
        collider->Reattach();
    }
}

} //namespace a2d
