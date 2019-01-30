//
// Created by selya on 25.12.2018.
//

#ifndef A2D_PHYSICS_COLLIDER_H
#define A2D_PHYSICS_COLLIDER_H

#include <a2d/core/component.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/physics/rigidbody.hpp>

#include <memory>

namespace a2d {

DECLARE_SMART_POINTER(PhysicsCollider)

class PhysicsCollider : public Component {
    friend class Rigidbody;
    friend class Object2D;

public:
    void SetDensity(float density) {
        if (this->density == density) return;
        this->density = density;
        Reattach();
    }

    void SetFriction(float friction) {
        if (this->friction == friction) return;
        this->friction = friction;
        Reattach();
    }

    void SetRestitution(float restitution) {
        if (this->restitution == restitution) return;
        this->restitution = restitution;
        Reattach();
    }

    float GetDensity() { return density; }
    float GetFriction() { return friction; }
    float GetRestitution() { return restitution; }

    pRigidbody GetRigidbody() {
        if (!fixture) return nullptr;
        return (Rigidbody *)fixture->GetBody()->GetUserData();
    }

protected:
    float density = 1.0f;
    float friction = 1.0f;
    float restitution = 0.0f;
    b2Fixture *fixture = nullptr;

    virtual std::shared_ptr<b2Shape> CalculateShape(const Vector2f &position, float rotation) = 0;

    virtual void AttachToRigidbody(b2Body *body) {
        if (fixture || !body) return;
        b2FixtureDef fixture_def;
        fixture_def.density = density;
        fixture_def.friction = friction;
        fixture_def.restitution = restitution;
        auto o = ((Rigidbody *)body->GetUserData())->GetObject2D();
        auto shape = CalculateShape(
                GetObject2D()->GetRelativePosition(o), GetObject2D()->GetRelativeRotation(o));
        fixture_def.shape = shape.get();
        fixture_def.userData = this;
        fixture = body->CreateFixture(&fixture_def);
    }

    virtual b2Body *DetachFromRigidbody() {
        if (!fixture) return nullptr;
        auto body = fixture->GetBody();
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;
        return body;
    }

    virtual void Reattach() {
        AttachToRigidbody(DetachFromRigidbody());
    }

    void FindAndAttach() {
        auto o = GetObject2D();
        while (o) {
            auto c = o->GetComponent<Rigidbody>();
            if (c) {
                AttachToRigidbody(c->body);
                break;
            }
            o = o->GetParent();
        }
    }

    void OnAttach() override {
        FindAndAttach();
    }

    void OnDetach() override {
        DetachFromRigidbody();
    }
};

} //namespace a2d

#endif //A2D_PHYSICS_COLLIDER_H
