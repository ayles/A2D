//
// Created by selya on 19.12.2018.
//

#ifndef A2D_RIGIDBODY_H
#define A2D_RIGIDBODY_H

#include <a2d/core/component.h>
#include <a2d/core/physics.h>
#include <a2d/core/object2d.h>
#include <a2d/core/components/line.h>

namespace a2d {

DECLARE_SMART_POINTER(Rigidbody)

class PhysicsBody : public Component {
    b2Body *body;

public:
    void Initialize() override {
        b2BodyDef body_def;
        body_def.type = b2_dynamicBody;
        body_def.position.Set(GetObject2D()->position.x, GetObject2D()->position.y);
        body = Physics::GetWorld().CreateBody(&body_def);
        b2PolygonShape p_shape;
        p_shape.SetAsBox(GetObject2D()->GetDrawable()->GetWidth() / 2 * GetObject2D()->scale.x,
                GetObject2D()->GetDrawable()->GetHeight() / 2 * GetObject2D()->scale.y);

        auto line = GetObject2D()->AddChild(new Object2D);
        auto &v = line->AddComponent<Line>()->vertices;

        for (int i = 0; i < 4; ++i) {
            v.emplace_back(Vector2f(p_shape.m_vertices[i].x / GetObject2D()->scale.x, p_shape.m_vertices[i].y / GetObject2D()->scale.y), Vector4f(1, 0, 0, 1));
        }
        v.emplace_back(Vector2f(p_shape.m_vertices[0].x / GetObject2D()->scale.x, p_shape.m_vertices[0].y / GetObject2D()->scale.y), Vector4f(1, 0, 0, 1));

        body->CreateFixture(&p_shape, 1.0f / GetObject2D()->GetDrawable()->GetWidth() / GetObject2D()->GetDrawable()->GetWidth());
    }

    void PhysicsUpdate() override {
        GetObject2D()->position.Set(body->GetPosition().x, body->GetPosition().y);
        GetObject2D()->rotation = body->GetAngle();
    };

    void PostUpdate() override {
        body->SetTransform(b2Vec2(GetObject2D()->position.x, GetObject2D()->position.y), GetObject2D()->rotation);
    }
};

} //namespace a2d

#endif //A2D_RIGIDBODY_H
