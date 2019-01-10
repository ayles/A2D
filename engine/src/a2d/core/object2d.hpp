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
#include <a2d/graphics/drawable.hpp>

#include <list>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <vector>

namespace a2d {

DECLARE_SMART_POINTER(Object2D)

class Object2D final : public ref_counter {
    friend class Engine;
    friend class Renderer;
    friend class Physics;
    friend class Component;

    struct objects_compare {
        bool operator()(const pObject2D &lhs, const pObject2D &rhs) const;
    };

    Matrix4f transform_matrix;
    pObject2D parent;
    std::list<pObject2D> children;
    std::list<pDrawable> drawables;
    std::unordered_map<std::type_index, std::list<pComponent>> components;

public:
    int layer;
    Vector2f position;
    Vector2f scale;
    float rotation;

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Object2D)
    Object2D();
    ~Object2D() override;

    pObject2D GetParent() const;
    const a2d::Matrix4f &GetTransformMatrix() const;
    const a2d::Matrix4f &GetTransformMatrixRecalculated(bool recursive = true);

    pObject2D AddChild(const pObject2D &child);

    void Destroy();

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, intrusive_ptr<T>>::type
    AddComponent();

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, intrusive_ptr<T>>::type
    GetComponent(bool look_for_base = false) const;

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, std::list<intrusive_ptr<T>>>::type
    GetComponents(bool look_for_base = false) const;

    void DestroyAllComponents();

    Vector2f WorldToLocal(const Vector2f &world_point);
    Vector2f LocalToWorld(const Vector2f &local_point);

private:
    void Draw(const a2d::Matrix4f &parent_transform, SpriteBatch &sprite_batch);
};


//
// Template implementation
//

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, intrusive_ptr<T>>::type
Object2D::AddComponent() {
    std::type_index t_index = typeid(T);
    intrusive_ptr<Component> component = new T;
    components[t_index].push_back(component);
    component->object_2d = this;

    auto drawable = dynamic_cast<Drawable *>(component.get());
    if (drawable) drawables.emplace_back(drawable);

    Engine::AddCommand([component]() {
        component->Initialize();
        if (Engine::IsPlaying()) component->OnResume();
        Engine::components.emplace_back(component);
    });

    return component;
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, intrusive_ptr<T>>::type
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
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, std::list<intrusive_ptr<T>>>::type
Object2D::GetComponents(bool look_for_base) const {
    if (look_for_base) {
        std::list<intrusive_ptr<T>> s;
        for (auto &i1 : components) {
            for (auto &i2 : i1.second) {
                T *t = dynamic_cast<T *>(i2.get());
                if (t) {
                    s.emplace_back(t);
                }
            }
        }
        return s;
    } else {
        auto iter = components.find(typeid(T));
        if (iter == components.end()) return std::list<intrusive_ptr<T> >();
        std::list<intrusive_ptr<T>> s(iter->second.begin(), iter->second.end());
        return s;
    }
}

} //namespace a2d

#endif //A2D_OBJECT2D_H
