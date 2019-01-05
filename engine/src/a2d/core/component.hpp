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
    friend Object2D;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Component)

    virtual SMART_POINTER(Object2D) GetObject2D() const;

    virtual bool IsActive() const;

protected:
    bool is_active;
    SMART_POINTER(Object2D) object_2d;

    virtual void SetActive(bool active);

    // Events
    virtual void Initialize() {};
    virtual void OnEnable() {};
    virtual void PhysicsUpdate() {};
    virtual void Update() {};
    virtual void PostUpdate() {};
    virtual void PreDraw() {};
    virtual void PostDraw() {};
    virtual void OnDisable() {};
    virtual void OnDestroy() {};
    virtual void OnPause() {};
    virtual void OnResume() {};

    ~Component() override {};
    Component();
};

} // namespace a2d

#endif //A2D_COMPONENT_H
