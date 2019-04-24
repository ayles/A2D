//
// Created by selya on 25.12.2018.
//

#ifndef A2D_PHYSICS_COLLIDER_HPP
#define A2D_PHYSICS_COLLIDER_HPP

#include <a2d/core/component.hpp>

#include <memory>

namespace a2d {

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

    intrusive_ptr<Rigidbody> GetRigidbody() {
        if (!fixture) return nullptr;
        return (Rigidbody *)fixture->GetBody()->GetUserData();
    }

protected:
    float density = 1.0f;
    float friction = 1.0f;
    float restitution = 0.0f;
    b2Fixture *fixture = nullptr;

    virtual b2Shape *CalculateShape(b2Body *body) = 0;

    virtual void AttachToRigidbody(b2Body *body) {
        if (fixture || !body || !IsActiveTransitive()) return;
        b2FixtureDef fixture_def;
        fixture_def.density = density * 0.01f * Physics::world_scale_inverted * Physics::world_scale_inverted;
        fixture_def.friction = friction;
        fixture_def.restitution = restitution;
        fixture_def.shape = CalculateShape(body);
        if (!fixture_def.shape) return;
        fixture_def.userData = this;
        fixture = body->CreateFixture(&fixture_def);
    }

    virtual b2Body *DetachFromRigidbody() {
        if (!fixture) return nullptr;
        auto body = fixture->GetBody();
        if (!body) return body;
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;
        return body;
    }

    virtual void Reattach() {
        auto body = DetachFromRigidbody();
        if (!body) FindAndAttach();
        else AttachToRigidbody(body);
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

    void OnEnable() override {
        //if (fixture) fixture->SetSensor(false);
    }

    void OnDisable() override {
        //if (fixture) fixture->SetSensor(true);
    }

    void OnAttach() override {
        FindAndAttach();
    }

    void OnDetach() override {
        DetachFromRigidbody();
    }
};

} //namespace a2d

#endif //A2D_PHYSICS_COLLIDER_HPP
