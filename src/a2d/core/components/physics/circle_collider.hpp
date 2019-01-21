//
// Created by selya on 25.12.2018.
//

#ifndef A2D_CIRCLE_COLLIDER_H
#define A2D_CIRCLE_COLLIDER_H

#include <a2d/core/components/physics/physics_collider.hpp>

namespace a2d {

class CircleCollider : public PhysicsCollider {
public:
    float radius = 1.0f;

private:
    b2Fixture *fixture = nullptr;
    intrusive_ptr<Line> line;

protected:

    void AttachToBody(const intrusive_ptr<PhysicsBody> &body) override {
        PhysicsCollider::AttachToBody(body);

        if (!fixture) {
            b2CircleShape circle;
            circle.m_radius = radius;
            fixture = body->body->CreateFixture(&circle, 1);
        }
    }

    void DetachFromBody(const intrusive_ptr<PhysicsBody> &body) override {
        PhysicsCollider::DetachFromBody(body);

        if (fixture) {
            body->body->DestroyFixture(fixture);
            fixture = nullptr;
        }
    }

    void Initialize() override {
        PhysicsCollider::Initialize();

        auto c = GetObject2D()->AddChild(new Object2D);
        line = c->AddComponent<Line>();
    }

    void PhysicsUpdate() override {
        PhysicsCollider::PhysicsUpdate();

        line->vertices.clear();

        int v = 16;
        for (int i = 0; i < v; ++i) {
            float angle = (float)i / v * 2 * 3.14159265358979323846f;
            line->vertices.emplace_back(Vector2f(std::cos(angle) * radius, std::sin(angle) * radius), Vector4f(0, 1, 0, 1));
        }

        line->vertices.push_back(line->vertices[0]);
        line->vertices.emplace_back(Vector2f(), Vector4f(0, 1, 0, 1));
    }
};

} //namespace a2d

#endif //A2D_CIRCLE_COLLIDER_H
