//
// Created by selya on 26.10.2018.
//

#ifndef A2D_COMPONENT_H
#define A2D_COMPONENT_H

#include <a2d/core/macro.hpp>
#include <a2d/core/ref_counter.hpp>

namespace a2d {

class Object2D;

DECLARE_SMART_POINTER(Component)

class Component : public ref_counter {
    friend class Object2D;
    friend class Engine;
    friend class Physics;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Component)

    virtual intrusive_ptr<Object2D> GetObject2D() const;
    virtual void Destroy() final;

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
};

} // namespace a2d

#endif //A2D_COMPONENT_H
