//
// Created by selya on 26.10.2018.
//

#ifndef A2D_OBJECT2D_HPP
#define A2D_OBJECT2D_HPP

#include <a2d/math/vector.hpp>
#include <a2d/math/matrix.hpp>
#include <a2d/core/ref_counter.hpp>
#include <a2d/core/macro.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/component.hpp>
#include <a2d/core/log.hpp>
#include <a2d/core/animation/animation_curve.hpp>

#include <set>
#include <unordered_map>
#include <typeindex>
#include <type_traits>

namespace a2d {

class Drawable;
class Animation;
class SpriteBatch;

template<class A>
class Animator;

class Object2D final : public ref_counter {
    friend class Engine;
    friend class Renderer;
    friend class Camera;
    friend class Physics;
    friend class Drawable;
    friend class Component;

    // Hierarchy
    intrusive_ptr<Object2D> parent;
    std::list<intrusive_ptr<Object2D>> children;
    std::list<intrusive_ptr<Object2D>>::const_iterator iter_in_parent;
    std::unordered_map<std::type_index, std::set<intrusive_ptr<Component>>> components;

    // Transform
    Matrix4f transform_matrix;
    Vector2f local_position;
    Vector2f local_scale;
    float local_rotation;

    // Drawing
    int layer;
    Vector4f color;

    // Helpers
    bool active;
    bool active_transitive;
    std::string tag;
    intrusive_ptr<Drawable> drawable;

    struct compare_objects {
        bool operator()(const intrusive_ptr<Object2D> &lhs, const intrusive_ptr<Object2D> &rhs) const;
    };

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Object2D)

    // Getters

    intrusive_ptr<Object2D> GetParent() const;
    const std::list<intrusive_ptr<Object2D>> &GetChildren() const;

    const a2d::Matrix4f &GetTransformMatrix() const;
    const Vector2f &GetLocalPosition() const;
    const Vector2f &GetLocalScale() const;
    float GetLocalRotation() const;
    Vector2f GetPosition() const;
    float GetRotation() const;
    Vector2f GetRelativePosition(const intrusive_ptr<Object2D> &origin) const;
    float GetRelativeRotation(const intrusive_ptr<Object2D> &origin) const;
    Vector2f WorldToLocalPoint(const Vector2f &world_point);
    Vector2f LocalToWorldPoint(const Vector2f &local_point);

    int GetLayer() const;
    const Vector4f &GetColor() const;

    bool IsActive() const;
    bool IsActiveTransitive() const;
    const std::string &GetTag() const;
    intrusive_ptr<Drawable> GetDrawable() const;
    template<class T>
    typename std::enable_if<std::is_base_of<Drawable, T>::value, intrusive_ptr<T>>::type
    GetDrawableAs() const;

    // Setters

    void SetLocalPosition(float x, float y);
    void SetLocalPosition(const Vector2f &position);
    void SetLocalScale(float x, float y);
    void SetLocalScale(const Vector2f &scale);
    void SetLocalRotation(float rotation);
    void SetPosition(float x, float y);
    void SetPosition(const Vector2f &position);
    void SetRotation(float rotation);

    void SetLayer(int layer);
    void SetColor(float r, float g, float b, float a);
    void SetColor(const Vector4f &color);

    void SetActive(bool active);
    void SetTag(const std::string &tag);

    // Manage

    void Attach(const intrusive_ptr<Object2D> &parent);
    void Destroy();
    void DestroyAllComponents();

    template<class Anim>
    typename std::enable_if<std::is_base_of<Animation, Anim>::value, intrusive_ptr<Animator<Anim>>>::type
    AddAnimation(const Anim &animation, float duration, int times = 1, bool mirror = false, float delay = 0.0f,
                 bool start_paused = false, AnimationCurve curve = AnimationCurve::Linear);

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, intrusive_ptr<T>>::type
    AddComponent();

    template<class T, class ...Args>
    typename std::enable_if<std::is_base_of<Component, T>::value, intrusive_ptr<T>>::type
    AddComponent(Args && ...args);

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, intrusive_ptr<T>>::type
    GetComponent(bool look_for_base = false) const;

    template<class T>
    typename std::enable_if<std::is_base_of<Component, T>::value, std::set<intrusive_ptr<T>>>::type
    GetComponents(bool look_for_base = false) const;

    static intrusive_ptr<Object2D> Create();

private:
    Object2D();
    ~Object2D() override;

    // Draw
    void Draw(SpriteBatch &sprite_batch, const Vector4f &parent_color = Vector4f(1));

    // Events
    void OnAttach();
    void OnDetach();
    void OnTransform(const intrusive_ptr<Object2D> &object, bool apply_local = true, bool scaling = false);
};


//
// Template implementation
//

template<class T>
typename std::enable_if<std::is_base_of<Drawable, T>::value, intrusive_ptr<T>>::type Object2D::GetDrawableAs() const {
    return dynamic_cast<T *>(drawable.get());
}

template<class Anim>
typename std::enable_if<std::is_base_of<Animation, Anim>::value, intrusive_ptr<Animator<Anim>>>::type
Object2D::AddAnimation(const Anim &animation, float duration, int times, bool mirror, float delay, bool start_paused,
             AnimationCurve curve) {
    return AddComponent<Animator<Anim>>(animation, duration, times, mirror, delay, start_paused, curve);
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, intrusive_ptr<T>>::type
Object2D::AddComponent() {
    std::type_index t_index = typeid(T);
    intrusive_ptr<Component> component = new T;
    components[t_index].emplace(component);
    component->object_2d = this;

    //Engine::AddCommand([component]() {
    component->Initialize();
    component->initialized = true;
    component->OnAttach();
    component->SetActive(true);
    if (Engine::IsPlaying()) component->OnResume();
    Engine::components.emplace(component);
    //});

    return component;
}


template<class T, class ...Args>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, intrusive_ptr<T>>::type
Object2D::AddComponent(Args && ...args) {
    std::type_index t_index = typeid(T);
    intrusive_ptr<Component> component = new T(std::forward<Args>(args)...);
    components[t_index].emplace(component);
    component->object_2d = this;

    //Engine::AddCommand([component]() {
    component->Initialize();
    component->initialized = true;
    component->OnAttach();
    component->SetActive(true);
    if (Engine::IsPlaying()) component->OnResume();
    Engine::components.emplace(component);
    //});

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
        //LOG_TRACE("Can't find component");
        return nullptr;
    } else {
        if (components.empty()) {
            //LOG_TRACE("Can't find component");
            return nullptr;
        }
        auto iter = components.find(typeid(T));
        if (iter == components.end() || iter->second.empty()) {
            //LOG_TRACE("Can't find component");
            return nullptr;
        }
        return *iter->second.begin();
    }
}

template<class T>
typename std::enable_if<std::is_base_of<a2d::Component, T>::value, std::set<intrusive_ptr<T>>>::type
Object2D::GetComponents(bool look_for_base) const {
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

#endif //A2D_OBJECT2D_HPP
