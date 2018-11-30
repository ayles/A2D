//
// Created by selya on 26.10.2018.
//

#ifndef A2D_OBJECT2D_H
#define A2D_OBJECT2D_H

#include <a2d/math/vector.h>
#include <a2d/math/matrix.h>
#include <a2d/core/ref_counter.h>
#include <a2d/core/macro.h>
#include <a2d/core/engine.h>
#include <a2d/core/component.h>
#include <a2d/core/drawable.h>

#include <set>
#include <map>
#include <typeindex>
#include <type_traits>

namespace a2d {

DECLARE_SMART_POINTER(Object2D)

class Object2D final : public ref_counter {
    friend class Engine;
    friend class Renderer;

    pObject2D parent;
    int layer;
    bool is_active;
    Matrix4f transform_matrix;
    pDrawable drawable;

    struct objects_compare {
        bool operator()(const pObject2D &lhs, const pObject2D &rhs) const;
    };

    std::map<std::type_index, std::set<SMART_POINTER(Component)>> components;

public:
    Vector2f position;
    Vector2f scale;
    float rotation;

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Object2D)

    Object2D();

    void SetLayer(int layer);

    bool IsActive();
    const a2d::Matrix4f &GetTransformMatrix() const;
    int GetLayer();
    pObject2D GetParent();

    pObject2D AddChild(pObject2D child);
    pObject2D RemoveChild(const pObject2D &child);

    template<class T>
    typename std::enable_if<std::is_base_of<a2d::Component, T>::value, SMART_POINTER(T)>::type
    AddComponent();

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, SMART_POINTER(T)>::type
    GetComponent() const;

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, std::set<SMART_POINTER(T)>>::type
    GetComponents() const;

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, void>::type
    RemoveComponent();

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, void>::type
    RemoveComponent(const SMART_POINTER(T) &component);

    ~Object2D() override;

    std::set<pObject2D, objects_compare> children;
private:
    void SetActive(bool active);

    void Update();
    void PostUpdate();
    void PreDraw(const a2d::Matrix4f &mat);
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
    component->SetActive(IsActive());
    if (a2d::Engine::IsPlaying()) component->OnResume();

    auto drawable = dynamic_cast<Drawable *>(component.get());
    if (drawable) {
        if (this->drawable) Engine::GetLogger()->error("More than one Drawable on object");
        if (parent) parent->children.erase(this);
        this->drawable = drawable;
        if (parent) parent->children.emplace(this);
    }

    return component;
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, SMART_POINTER(T)>::type
Object2D::GetComponent() const {
    auto iter = components.find(typeid(T));
    if (iter == components.end() || iter->second.empty()) return nullptr;
    return *iter->second.begin();
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, std::set<SMART_POINTER(T)>>::type
Object2D::GetComponents() const {
    auto iter = components.find(typeid(T));
    if (iter == components.end()) return std::set<SMART_POINTER(T)>();
    std::set<SMART_POINTER(T)> s(iter->second.begin(), iter->second.end());
    return s;
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
    iter->second.erase(component);
    if (component == drawable) {
        if (parent) parent->children.erase(this);
        drawable = nullptr;
        if (parent) parent->children.emplace(this);
    }
}



} //namespace a2d

#endif //A2D_OBJECT2D_H
