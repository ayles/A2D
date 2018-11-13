//
// Created by selya on 26.10.2018.
//

#ifndef A2D_OBJECT2D_H
#define A2D_OBJECT2D_H

#include <a2d/math/vector.h>
#include <a2d/math/matrix.h>
#include <a2d/core/ref_counter.h>
#include <a2d/core/macro.h>
#include <a2d/core/component.h>

#include <set>
#include <map>
#include <typeindex>
#include <type_traits>
#include <iostream>

namespace a2d {

DECLARE_SMART_POINTER(Object2D)

class Object2D final : public ref_counter {
public:
    friend class Engine;

    Vector2f position;
    Vector2f scale;
    float rotation;

    Object2D();

    bool IsActive();

    const a2d::Matrix4f &GetTransformMatrix() const;

    int GetLayer();
    void SetLayer(int layer);

    pObject2D GetParent();

    pObject2D AddChild(pObject2D child);
    pObject2D RemoveChild(const pObject2D &child);

    template<class T>
    typename std::enable_if<std::is_base_of<a2d::Component, T>::value, SMART_POINTER(T)>::type
    AddComponent() {
        std::type_index t_index = typeid(T);
        SMART_POINTER(T) component = new T;
        if (components.find(t_index) != components.end()) {
            components[t_index].insert(component);
        } else {
            (components[t_index] = std::set<pComponent>()).insert(component);
        }
        component->object_2d = this;
        component->Initialize();
        component->SetActive(IsActive());
        return component;
    }

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, SMART_POINTER(T)>::type
    GetComponent() const {
        auto c = components.find(typeid(T));
        if (c == components.end() || c->second.empty()) return nullptr;
        return dynamic_cast<T *>(c->second.begin()->get());
    }

    virtual ~Object2D();

private:
    pObject2D parent;
    int layer;
    bool is_active;
    Matrix4f transform_matrix;

    std::set<pObject2D> children;
    std::map<std::type_index, std::set<pComponent>> components;

    void SetActive(bool active);

    void LayerHierarchyAboutToChange();
    void LayerHierarchyChanged();
    void Update();
    void PostUpdate();
    void PreDraw(const a2d::Matrix4f &mat);
    void PostDraw();
};

} //namespace a2d

#endif //A2D_OBJECT2D_H
