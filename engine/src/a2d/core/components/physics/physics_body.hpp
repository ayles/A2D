//
// Created by selya on 19.12.2018.
//

#ifndef A2D_PHYSICS_BODY_H
#define A2D_PHYSICS_BODY_H

#include <a2d/core/component.hpp>
#include <a2d/core/physics.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/components/line.hpp>

namespace a2d {

DECLARE_SMART_POINTER(PhysicsBody)

class PhysicsBody : public Component {
    friend class PhysicsCollider;

public:
    enum BodyType {
        DYNAMIC = b2_dynamicBody,
        KINEMATIC = b2_kinematicBody,
        STATIC = b2_staticBody,
    };

//private:

    b2Body *body;

public:

    void SetType(BodyType body_type) {
        body->SetType((b2BodyType) body_type);
    }

    BodyType GetType() {
        return (BodyType) body->GetType();
    }

    void SetMass(float mass) {
        b2MassData mass_data;
        body->GetMassData(&mass_data);
        mass_data.mass = mass;
        body->SetMassData(&mass_data);
    }

    float GetMass() {
        b2MassData mass_data;
        body->GetMassData(&mass_data);
        return mass_data.mass;
    }

    void SetInertia(float inertia) {
        b2MassData mass_data;
        body->GetMassData(&mass_data);
        mass_data.I = inertia;
        body->SetMassData(&mass_data);
    }

    float GetInertia() {
        b2MassData mass_data;
        body->GetMassData(&mass_data);
        return mass_data.I;
    }

    void SetCenterOfMass(const Vector2f &center_of_mass) {
        b2MassData mass_data;
        body->GetMassData(&mass_data);
        mass_data.center.Set(center_of_mass.x, center_of_mass.y);
        body->SetMassData(&mass_data);
    }

    Vector2f GetCenterOfMass() {
        b2MassData mass_data;
        body->GetMassData(&mass_data);
        return Vector2f(mass_data.center.x, mass_data.center.y);
    }

    void SetPosition(const Vector2f &position) {
        const_cast<b2Transform &>(body->GetTransform()).p.Set(position.x, position.y);
    }

    Vector2f GetPosition() {
        auto &p = body->GetTransform().p;
        return Vector2f(p.x, p.y);
    }

    void SetRotation(float rotation) {
        const_cast<b2Transform &>(body->GetTransform()).q.Set(rotation);
    }

    float GetRotation() {
        return body->GetTransform().q.GetAngle();
    }

private:

    void Initialize() override {
        b2BodyDef body_def;
        body_def.type = b2_dynamicBody;
        body_def.position.Set(GetObject2D()->position.x, GetObject2D()->position.y);
        body_def.angle = GetObject2D()->rotation;
        body = Physics::GetWorld().CreateBody(&body_def);
    }

    void OnDestroy() override {
        Physics::GetWorld().DestroyBody(body);
    }

    void PhysicsUpdate() override {
        GetObject2D()->position.Set(body->GetPosition().x, body->GetPosition().y);
        GetObject2D()->rotation = body->GetAngle();
    }
};

} //namespace a2d

#endif //A2D_PHYSICS_BODY_H
