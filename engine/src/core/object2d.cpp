//
// Created by selya on 04.11.2018.
//

#include <A2D/core/object2d.h>

namespace a2d {

Object2D::Object2D() : is_active(false), layer(0), parent(nullptr), scale(1), position(), rotation() {

}

bool Object2D::IsActive() {
    return is_active;
}

void Object2D::SetActive(bool active) {
    if (is_active == active) return;
    is_active = active;
    for (std::pair<std::type_index, std::set<pComponent>> s : components) {
        for (const pComponent &c : s.second) {
            c->SetActive(is_active);
        }
    }
    for (const pObject2D &c : children) {
        c->SetActive(is_active);
    }
}

const Matrix4f &Object2D::GetTransformMatrix() const {
    return transform_matrix;
}

int Object2D::GetLayer() {
    return layer;
}

void Object2D::SetLayer(int layer) {
    if (layer == this->layer) return;
    this->LayerHierarchyAboutToChange();
    this->layer = layer;
    this->LayerHierarchyChanged();
}

pObject2D Object2D::GetParent() {
    return parent;
}

pObject2D Object2D::AddChild(pObject2D child) {
    if (child->parent != this) {
        if (child->parent != nullptr) child->parent->RemoveChild(child);
        child->parent = this;
        children.insert(child);
        child->SetActive(true);
    }
    return child;
}

pObject2D Object2D::RemoveChild(const pObject2D &child) {
    if (child->parent == this) {
        child->SetActive(false);
        child->parent = nullptr;
        children.erase(child);
    }
    return child;
}

void Object2D::LayerHierarchyAboutToChange() {
    for (std::pair<std::type_index, std::set<pComponent>> s : components) {
        for (const pComponent &c : s.second) {
            c->LayerHierarchyAboutToChange();
        }
    }
    for (const pObject2D &c : children) {
        c->LayerHierarchyAboutToChange();
    }
}

void Object2D::LayerHierarchyChanged() {
    for (std::pair<std::type_index, std::set<pComponent>> s : components) {
        for (const pComponent &c : s.second) {
            c->LayerHierarchyChanged();
        }
    }
    for (const pObject2D &c : children) {
        c->LayerHierarchyChanged();
    }
}

void Object2D::Update() {
    for (std::pair<std::type_index, std::set<pComponent>> s : components) {
        for (const pComponent &c : s.second) {
            c->Update();
        }
    }
    for (const pObject2D &c : children) {
        c->Update();
    }
}

void Object2D::PostUpdate() {
    for (std::pair<std::type_index, std::set<pComponent>> s : components) {
        for (const pComponent &c : s.second) {
            c->PostUpdate();
        }
    }
    for (const pObject2D &c : children) {
        c->PostUpdate();
    }
}

void Object2D::PreDraw(const a2d::Matrix4f &mat) {
    transform_matrix.Identity();
    transform_matrix.Translate(position.x, position.y, 0.0f);
    transform_matrix.Rotate(rotation, 0.0f, 0.0f, 1.0f);
    transform_matrix.Scale(scale.x, scale.y, 1.0f);

    transform_matrix = mat * transform_matrix;

    for (std::pair<std::type_index, std::set<pComponent>> s : components) {
        for (const pComponent &c : s.second) {
            c->PreDraw();
        }
    }

    for (const pObject2D &c : children) {
        c->PreDraw(transform_matrix);
    }
}


void Object2D::PostDraw() {
    for (std::pair<std::type_index, std::set<pComponent>> s : components) {
        for (const pComponent &c : s.second) {
            c->PostDraw();
        }
    }
    for (const pObject2D &c : children) {
        c->PostDraw();
    }
}

Object2D::~Object2D() {

}

} //namespace a2d
