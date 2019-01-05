//
// Created by selya on 04.11.2018.
//

#include <a2d/core/object2d.hpp>
#include <a2d/core/component.hpp>

namespace a2d {

Object2D::Object2D() : is_in_tree(false), layer(0), parent(nullptr), scale(1), position(), rotation() {

}

void Object2D::SetLayer(int layer) {
    if (layer == this->layer) return;
    if (parent) parent->children.erase(this);
    this->layer = layer;
    if (parent) parent->children.emplace(this);
}

pObject2D Object2D::GetParent() const {
    return parent;
}

int Object2D::GetLayer() const {
    return layer;
}

bool Object2D::IsInTree() const {
    return is_in_tree;
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

void Object2D::SetIsInTree(bool is_in_tree) {
    if (this->is_in_tree == is_in_tree) return;
    this->is_in_tree = is_in_tree;
    for (const auto &s : components) {
        for (const pComponent &c : s.second) {
            c->SetActive(is_in_tree);
        }
    }
    for (const pObject2D &c : children) {
        c->SetIsInTree(is_in_tree);
    }
}

pObject2D Object2D::AddChild(pObject2D child) {
    if (child->parent != this) {
        if (child->parent) child->parent->RemoveChild(child);
        child->parent = this;
        children.emplace(child);
        child->SetIsInTree(true);
    }
    return child;
}

pObject2D Object2D::RemoveChild(const pObject2D &child) {
    if (child->parent == this) {
        child->SetIsInTree(false);
        child->parent = nullptr;
        children.erase(child);
    }
    return child;
}

void Object2D::PhysicsUpdate() {
    for (const auto &s : components) {
        for (const pComponent &c : s.second) {
            c->PhysicsUpdate();
        }
    }
    for (const pObject2D &c : children) {
        c->PhysicsUpdate();
    }
}

void Object2D::Update() {
    for (const auto &s : components) {
        for (const pComponent &c : s.second) {
            c->Update();
        }
    }
    for (const pObject2D &c : children) {
        c->Update();
    }
}

void Object2D::PostUpdate() {
    for (const auto &s : components) {
        for (const pComponent &c : s.second) {
            c->PostUpdate();
        }
    }
    for (const pObject2D &c : children) {
        c->PostUpdate();
    }
}

void Object2D::PreDraw(const a2d::Matrix4f &parent_transform) {
    GetTransformMatrixRecalculated(false);

    for (const auto &s : components) {
        for (const pComponent &c : s.second) {
            c->PreDraw();
        }
    }

    for (const pObject2D &c : children) {
        c->PreDraw(transform_matrix);
    }
}

void Object2D::Draw(SpriteBatch &sprite_batch) {
    for (auto &drawable : drawables) {
        drawable->Draw(sprite_batch);
    }
    for (const pObject2D &c : children) {
        c->Draw(sprite_batch);
    }
}

void Object2D::PostDraw() {
    for (const auto &s : components) {
        for (const pComponent &c : s.second) {
            c->PostDraw();
        }
    }
    for (const pObject2D &c : children) {
        c->PostDraw();
    }
}

void Object2D::OnPause() {
    for (const auto &s : components) {
        for (const pComponent &c : s.second) {
            c->OnPause();
        }
    }
    for (const pObject2D &c : children) {
        c->OnPause();
    }
}

void Object2D::OnResume() {
    for (const auto &s : components) {
        for (const pComponent &c : s.second) {
            c->OnResume();
        }
    }
    for (const pObject2D &c : children) {
        c->OnResume();
    }
}

void Object2D::CleanTree() {
    for (const auto &s : components) {
        while (!s.second.empty()) RemoveComponent(*s.second.begin());
    }
    for (const pObject2D &c : children) {
        c->CleanTree();
    }
}

Object2D::~Object2D() {

}

bool Object2D::objects_compare::operator()(const pObject2D &lhs, const pObject2D &rhs) const {
    if (lhs->GetLayer() != rhs->GetLayer()) return lhs->GetLayer() < rhs->GetLayer();
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

Vector2f Object2D::WorldToLocal(const Vector2f &world_point) {
    auto v = Matrix4f(GetTransformMatrixRecalculated()).Inverse().Transform(world_point.x, world_point.y, 0, 1);
    return Vector2f(v.x, v.y);
}

Vector2f Object2D::LocalToWorld(const Vector2f &local_point) {
    auto v = GetTransformMatrixRecalculated().Transform(local_point.x, local_point.y, 0, 1);
    return Vector2f(v.x, v.y);
}

} //namespace a2d
