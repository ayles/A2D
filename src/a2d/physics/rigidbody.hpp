//
// Created by selya on 19.12.2018.
//

#ifndef A2D_RIGIDBODY_HPP
#define A2D_RIGIDBODY_HPP

#include <a2d/core/component.hpp>
#include <a2d/physics/physics.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/renderer/line.hpp>

namespace a2d {

DECLARE_SMART_POINTER(Rigidbody)

class Rigidbody : public Component {
    friend class PhysicsCollider;
    friend class CircleCollider;
    friend class BoxCollider;
    friend class Object2D;
    friend class Physics;

    b2Body *body;

public:
    enum BodyType {
        DYNAMIC = b2_dynamicBody,
        KINEMATIC = b2_kinematicBody,
        STATIC = b2_staticBody,
    };

    BodyType GetType();
    float GetMass();
    float GetInertia();
    Vector2f GetCenterOfMass();
    float GetGravityScale();
    Vector2f GetLinearVelocity();
    float GetAngularVelocity();
    float GetLinearDamping();
    float GetAngularDamping();
    bool IsFixedRotation();
    bool IsAwake();
    bool IsBullet();
    bool IsSleepingAllowed();

    void SetType(BodyType body_type);
    void SetMass(float mass);
    void SetInertia(float inertia);
    void SetCenterOfMass(const Vector2f &center_of_mass);
    void SetGravityScale(float gravity_scale);
    void SetLinearVelocity(const Vector2f &velocity);
    void SetAngularVelocity(float velocity);
    void SetLinearDamping(float damping);
    void SetAngularDamping(float damping);
    void SetFixedRotation(bool rotation_fixed);
    void SetAwake(bool awake);
    void SetBullet(bool bullet);
    void SetSleepingAllowed(bool sleeping_allowed);

    void ApplyForce(const Vector2f &force, const Vector2f &point, bool wake = true);
    void ApplyForceToCenter(const Vector2f &force, bool wake = true);
    void ApplyLinearImpulse(const Vector2f &impulse, const Vector2f &point, bool wake = true);
    void ApplyLinearImpulseToCenter(const Vector2f &impulse, bool wake = true);
    void ApplyAngularImpulse(float impulse, bool wake = true);
    void ApplyTorque(float torque, bool wake = true);

private:
    void Initialize() override;
    void OnEnable() override;
    void OnDisable() override;
    void OnDestroy() override;

    bool internal_transform = false;
    void Transform(const Vector2f &position, float rotation);

    void PhysicsStep();
};

} //namespace a2d

#endif //A2D_RIGIDBODY_HPP
