//
// Created by selya on 25.12.2018.
//

#ifndef A2D_CIRCLE_COLLIDER_H
#define A2D_CIRCLE_COLLIDER_H

#include <a2d/components/physics/physics_collider.hpp>
#include "rigidbody.hpp"


namespace a2d {

class CircleCollider : public PhysicsCollider {
private:
    float radius = 0.0f;

#ifndef NDEBUG
    intrusive_ptr<Line> line;
#endif

public:
    void SetRadius(float radius) {
        this->radius = radius;
        Reattach();

#ifndef NDEBUG
        if (!line) line = GetObject2D()->AddComponent<Line>();
        line->vertices.clear();
        int v = 16;
        for (int i = 0; i < v; ++i) {
            float angle = (float)i / v * 2 * 3.14159265358979323846f;
            line->vertices.emplace_back(Vector2f(std::cos(angle) * radius, std::sin(angle) * radius),
                                        Vector4f(0, 1, 0, 1));
        }
        line->vertices.push_back(line->vertices[0]);
        line->vertices.emplace_back(Vector2f(), Vector4f(0, 1, 0, 1));
        line->Load();
#endif
    }

    float GetRadius() {
        return radius;
    }

protected:
    std::shared_ptr<b2Shape> CalculateShape(const Vector2f &position, float rotation) override {
        auto circle_shape = new b2CircleShape;
        float s = Physics::GetWorldScale();
        circle_shape->m_radius = radius * s;
        circle_shape->m_p = b2Vec2(position.x * s, position.y * s);
        return std::shared_ptr<b2Shape>(circle_shape);
    };
};

} //namespace a2d

#endif //A2D_CIRCLE_COLLIDER_H
