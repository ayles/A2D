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
#include <a2d/core/log.hpp>

#include <set>
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
    friend class Drawable;
    friend class Component;

    struct compare_objects {
        bool operator()(const pObject2D &lhs, const pObject2D &rhs) const;
    };

    Matrix4f transform_matrix;
    pObject2D parent;
    std::set<pObject2D, compare_objects> children;
    std::set<pDrawable> drawables;
    std::unordered_map<std::type_index, std::set<pComponent>> components;
    int layer;

public:
    Vector2f position;
    Vector2f scale;
    float rotation;

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Object2D)

    int GetLayer();
    pObject2D GetParent() const;
    const a2d::Matrix4f &GetTransformMatrix() const;
    const a2d::Matrix4f &GetTransformMatrixRecalculated(bool recursive = true);

    void SetLayer(int layer);

    void Attach(const pObject2D &parent);

    void Destroy();

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, intrusive_ptr<T>>::type
    AddComponent();

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, intrusive_ptr<T>>::type
    GetComponent(bool look_for_base = false) const;

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, std::set<intrusive_ptr<T>>>::type
    GetComponents(bool look_for_base = false) const;

    void DestroyAllComponents();

    Vector2f WorldToLocal(const Vector2f &world_point);
    Vector2f LocalToWorld(const Vector2f &local_point);

    static pObject2D Create();

private:
    Object2D();
    ~Object2D() override;

    void Draw(const a2d::Matrix4f &parent_transform, SpriteBatch &sprite_batch);
};


//
// Template implementation
//

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, intrusive_ptr<T>>::type
Object2D::AddComponent() {
    ASSERT_MAIN_THREAD
    std::type_index t_index = typeid(T);
    intrusive_ptr<Component> component = new T;
    components[t_index].emplace(component);
    component->object_2d = this;

    Engine::AddCommand([component]() {
        component->Initialize();
        if (Engine::IsPlaying()) component->OnResume();
        Engine::components.emplace(component);
    });

    return component;
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, intrusive_ptr<T>>::type
Object2D::GetComponent(bool look_for_base) const {
    ASSERT_MAIN_THREAD
    if (look_for_base) {
        for (auto &i1 : components) {
            for (auto &i2 : i1.second) {
                T *t = dynamic_cast<T *>(i2.get());
                if (t) {
                    return t;
                }
            }
        }
        LOG_TRACE("Can't find component");
        return nullptr;
    } else {
        if (components.empty()) {
            LOG_TRACE("Can't find component");
            return nullptr;
        }
        auto iter = components.find(typeid(T));
        if (iter == components.end() || iter->second.empty()) {
            LOG_TRACE("Can't find component");
            return nullptr;
        }
        return *iter->second.begin();
    }
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, std::set<intrusive_ptr<T>>>::type
Object2D::GetComponents(bool look_for_base) const {
    ASSERT_MAIN_THREAD
    if (look_for_base) {
        std::set<intrusive_ptr<T>> s;
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
        if (iter == components.end()) return std::set<intrusive_ptr<T> >();
        std::set<intrusive_ptr<T>> s(iter->second.begin(), iter->second.end());
        return s;
    }
}

} //namespace a2d

#endif //A2D_OBJECT2D_H
