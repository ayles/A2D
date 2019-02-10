//
// Created by selya on 26.10.2018.
//

#ifndef A2D_COMPONENT_H
#define A2D_COMPONENT_H

#include <a2d/core/macro.hpp>
#include <a2d/core/ref_counter.hpp>
#include <a2d/physics/physics.hpp>

#include <list>

namespace a2d {

class Object2D;

DECLARE_SMART_POINTER(Component)

class Component : public ref_counter {
    friend class Object2D;
    friend class Engine;
    friend class Physics;

    bool initialized;
    bool active;
    bool active_transitive;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Component)

    virtual intrusive_ptr<Object2D> GetObject2D() const final;
    virtual void Destroy() final;
    virtual void SetActive(bool active) final;
    virtual bool IsActive() const final;
    virtual bool IsActiveTransitive() const final;

protected:
    Object2D *object_2d;

    Component();
    ~Component() override {};

    // Events
    virtual void Initialize() {};
    virtual void PhysicsUpdate() {};
    virtual void Update() {};
    virtual void PostUpdate() {};
    virtual void OnDestroy() {};
    virtual void OnPause() {};
    virtual void OnResume() {};
    virtual void OnAttach() {};
    virtual void OnDetach() {};
    virtual void OnEnable() {};
    virtual void OnDisable() {};
    virtual void OnCollisionBegin(const intrusive_ptr<CollisionContact> &contact) {};
    virtual void OnCollisionEnd(const intrusive_ptr<CollisionContact> &contact) {};
};

} // namespace a2d

#endif //A2D_COMPONENT_H
