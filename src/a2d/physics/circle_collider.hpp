//
// Created by selya on 25.12.2018.
//

#ifndef A2D_CIRCLE_COLLIDER_HPP
#define A2D_CIRCLE_COLLIDER_HPP

#include <a2d/physics/physics_collider.hpp>


namespace a2d {

class CircleCollider : public PhysicsCollider {
#ifndef NDEBUG
    intrusive_ptr<Line> line;
#endif

    float radius;
    b2CircleShape shape;

public:
    void SetRadius(float radius) {
        this->radius = radius;
        Reattach();
    }

    float GetRadius() {
        return radius;
    }

protected:
    b2Shape *CalculateShape(b2Body *body) override {
#ifndef NDEBUG
        if (!line) {
            auto o = Object2D::Create();
            o->Attach(GetObject2D());
            line = o->AddComponent<Line>();
        }
        line->vertices.clear();
        int v = 16;
        for (int i = 0; i < v; ++i) {
            float angle = (float)i / v * 2 * 3.14159265358979323846f;
            line->vertices.emplace_back(Vector2f(std::cos(angle) * radius, std::sin(angle) * radius),
                                        Vector4f(0, 1, 0, 1));
        }
        line->vertices.emplace_back(line->vertices[0]);
        line->vertices.emplace_back(Vector2f(), Vector4f(0, 1, 0, 1));
        line->Load();
#endif
        float r = (GetObject2D()->LocalToWorldPoint(Vector2f(radius, 0)) - GetObject2D()->GetPosition()).Length();
        auto position = GetObject2D()->GetRelativePosition(((Rigidbody *)body->GetUserData())->GetObject2D());
        // TODO scale
        float s = Physics::GetWorldScale();
        shape.m_radius = r * s;
        shape.m_p = b2Vec2(position.x, position.y);
        return &shape;
    };
};

} //namespace a2d

#endif //A2D_CIRCLE_COLLIDER_HPP
