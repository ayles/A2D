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

#include <set>
#include <map>
#include <typeindex>
#include <type_traits>
#include <iostream>

namespace a2d {

DECLARE_SMART_POINTER(Object2D)

class Component;

class Object2D final : public ref_counter {
    friend class Engine;

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
    AddComponent() {
        std::type_index t_index = typeid(T);
        SMART_POINTER(Component) component = new T;
        if (components.find(t_index) != components.end()) {
            components[t_index].insert(component);
        } else {
            (components[t_index] = std::set<SMART_POINTER(Component)>()).insert(component);
        }
        component->object_2d = this;
        component->Initialize();
        component->SetActive(IsActive());

        if (a2d::Engine::IsPlaying()) {
            component->OnResume();
        }

        return component;
    }

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, SMART_POINTER(T)>::type
    GetComponent() const {
        auto iter = components.find(typeid(T));
        if (iter == components.end() || iter->second.empty()) return nullptr;
        return *iter->second.begin();
    }

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, std::set<SMART_POINTER(T)>>::type
    GetComponents() const {
        auto iter = components.find(typeid(T));
        if (iter == components.end()) return std::set<SMART_POINTER(T)>();
        std::set<SMART_POINTER(T)> s;
        for (const auto &component : iter->second) {
            s.insert(component);
        }
        return s;
    }

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, void>::type
    RemoveComponent() {
        auto iter = components.find(typeid(T));
        if (iter == components.end() || iter->second.empty()) return;
        auto component = *iter->second.begin();
        if (a2d::Engine::IsPlaying()) {
            component->OnPause();
        }
        component->SetActive(false);
        component->OnDestroy();
        iter->second.erase(component);
    }

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, void>::type
    RemoveComponent(SMART_POINTER(T) component) {
        auto iter = components.find(typeid(*component));
        if (iter == components.end() || iter->second.empty()) return;
        if (iter->second.find(component) == iter->second.end()) return;
        if (a2d::Engine::IsPlaying()) {
            component->OnPause();
        }
        component->SetActive(false);
        component->OnDestroy();
        iter->second.erase(component);
    }

    virtual ~Object2D() override;

private:
    pObject2D parent;
    int layer;
    bool is_active;
    Matrix4f transform_matrix;

    std::set<pObject2D> children;
    std::map<std::type_index, std::set<SMART_POINTER(Component)>> components;

    void SetActive(bool active);

    void LayerHierarchyAboutToChange();
    void LayerHierarchyChanged();
    void Update();
    void PostUpdate();
    void PreDraw(const a2d::Matrix4f &mat);
    void PostDraw();
    void OnPause();
    void OnResume();
    void CleanTree();
};

} //namespace a2d

#endif //A2D_OBJECT2D_H
