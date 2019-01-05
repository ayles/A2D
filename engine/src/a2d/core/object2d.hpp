//
// Created by selya on 26.10.2018.
//

#ifndef A2D_OBJECT2D_H
#define A2D_OBJECT2D_H

#include <a2d/math/vector.hpp>
#include <a2d/math/matrix.hpp>
#include <a2d/core/ref_counter.hpp>
#include <a2d/core/macro.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/component.hpp>
#include <a2d/core/drawable.hpp>

#include <set>
#include <unordered_map>
#include <typeindex>
#include <type_traits>

#include "drawable.hpp"


namespace a2d {

DECLARE_SMART_POINTER(Object2D)

class Object2D final : public ref_counter {
    friend class Engine;
    friend class Renderer;
    friend class Physics;

    struct objects_compare {
        bool operator()(const pObject2D &lhs, const pObject2D &rhs) const;
    };

    std::set<pObject2D, objects_compare> children;
    pObject2D parent;
    int layer;
    bool is_in_tree;
    Matrix4f transform_matrix;
    std::set<pDrawable> drawables;
    std::unordered_map<std::type_index, std::set<SMART_POINTER(Component)>> components;

public:
    Vector2f position;
    Vector2f scale;
    float rotation;

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Object2D)

    Object2D();

    void SetLayer(int layer);

    pObject2D GetParent() const;
    int GetLayer() const;
    bool IsInTree() const;
    const a2d::Matrix4f &GetTransformMatrix() const;
    const a2d::Matrix4f &GetTransformMatrixRecalculated(bool recursive = true);

    pObject2D AddChild(pObject2D child);
    pObject2D RemoveChild(const pObject2D &child);

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, SMART_POINTER(T)>::type
    AddComponent();

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, SMART_POINTER(T)>::type
    GetComponent(bool look_for_base = false) const;

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, std::set<SMART_POINTER(T)>>::type
    GetComponents(bool look_for_base = false) const;

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, void>::type
    RemoveComponent();

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, void>::type
    RemoveComponent(const SMART_POINTER(T) &component);

    Vector2f WorldToLocal(const Vector2f &world_point);
    Vector2f LocalToWorld(const Vector2f &local_point);

    ~Object2D() override;

private:
    void SetIsInTree(bool is_in_tree);

    void PhysicsUpdate();
    void Update();
    void PostUpdate();
    void PreDraw(const a2d::Matrix4f &parent_transform);
    void Draw(SpriteBatch &sprite_batch);
    void PostDraw();
    void OnPause();
    void OnResume();
    void CleanTree();
};


//
// Template implementation
//

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, SMART_POINTER(T)>::type
Object2D::AddComponent() {
    std::type_index t_index = typeid(T);
    SMART_POINTER(Component) component = new T;
    components[t_index].emplace(component);
    component->object_2d = this;
    component->Initialize();
    component->SetActive(IsInTree());
    if (a2d::Engine::IsPlaying()) component->OnResume();

    auto drawable = dynamic_cast<Drawable *>(component.get());
    if (drawable) {
        if (parent) parent->children.erase(this);
        this->drawables.emplace(drawable);
        if (parent) parent->children.emplace(this);
    }

    return component;
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, SMART_POINTER(T)>::type
Object2D::GetComponent(bool look_for_base) const {
    if (look_for_base) {
        for (auto &i1 : components) {
            for (auto &i2 : i1.second) {
                T *t = dynamic_cast<T *>(i2.get());
                if (t) {
                    return t;
                }
            }
        }
        return nullptr;
    } else {
        auto iter = components.find(typeid(T));
        if (iter == components.end() || iter->second.empty()) return nullptr;
        return *iter->second.begin();
    }
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, std::set<SMART_POINTER(T)>>::type
Object2D::GetComponents(bool look_for_base) const {
    if (look_for_base) {
        std::set<SMART_POINTER(T)> s;
        for (auto &i1 : components) {
            for (auto &i2 : i1.second) {
                T *t = dynamic_cast<T *>(i2.get());
                if (t) {
                    s.emplace(t);
                }
            }
        }
        return s;
    } else {
        auto iter = components.find(typeid(T));
        if (iter == components.end()) return std::set<SMART_POINTER(T) >();
        std::set<SMART_POINTER(T)> s(iter->second.begin(), iter->second.end());
        return s;
    }
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, void>::type
Object2D::RemoveComponent() {
    auto iter = components.find(typeid(T));
    if (iter == components.end() || iter->second.empty()) return;
    RemoveComponent(*iter->second.begin());
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, void>::type
Object2D::RemoveComponent(const SMART_POINTER(T) &component) {
    auto iter = components.find(typeid(*component));
    if (iter == components.end() || iter->second.empty()) return;
    if (iter->second.find(component) == iter->second.end()) return;
    if (a2d::Engine::IsPlaying()) {
        component->OnPause();
    }
    component->SetActive(false);
    component->OnDestroy();

    auto d = drawables.find(component);
    if (d != drawables.end()) {
        if (parent) parent->children.erase(this);
        drawables.erase(d);
        if (parent) parent->children.emplace(this);
    }

    iter->second.erase(component);
}



} //namespace a2d

#endif //A2D_OBJECT2D_H
