//
// Created by selya on 26.01.2019.
//

#ifndef A2D_BOX_COLLIDER_H
#define A2D_BOX_COLLIDER_H

#include <a2d/components/physics/physics_collider.hpp>

namespace a2d {

class BoxCollider : public PhysicsCollider {
private:
    Vector2f half_size;

#ifndef NDEBUG
    intrusive_ptr<Line> line;
#endif

public:
    void SetHalfWidth(float half_width) {
        half_size.x = half_width;
        SetHalfSize(half_width, half_size.y);
    }

    void SetHalfHeight(float half_height) {
        half_size.y = half_height;
        SetHalfSize(half_size.x, half_height);
    }

    void SetHalfSize(const Vector2f &half_size) {
        SetHalfSize(half_size.x, half_size.y);
    }

    void SetHalfSize(float half_width, float half_height) {
        half_size.Set(half_width, half_height);
        Reattach();

#ifndef NDEBUG
        if (!line) line = GetObject2D()->AddComponent<Line>();
        line->vertices.clear();
        line->vertices.emplace_back(Vector2f(-half_size.x, -half_size.y), Vector4f(0, 1, 0, 1));
        line->vertices.emplace_back(Vector2f(half_size.x, -half_size.y), Vector4f(0, 1, 0, 1));
        line->vertices.emplace_back(Vector2f(half_size.x, half_size.y), Vector4f(0, 1, 0, 1));
        line->vertices.emplace_back(Vector2f(-half_size.x, half_size.y), Vector4f(0, 1, 0, 1));
        line->vertices.push_back(line->vertices[0]);
        line->Load();
#endif
    }

    Vector2f GetHalfSize() {
        return half_size;
    }

    float GetHalfWidth() {
        return half_size.x;
    }

    float GetHalfHeight() {
        return half_size.y;
    }

protected:
    std::shared_ptr<b2Shape> CalculateShape(const Vector2f &position, float rotation) override {
        auto box_shape = new b2PolygonShape;
        float s = Physics::GetWorldScale();
        box_shape->SetAsBox(half_size.x * s, half_size.y * s, b2Vec2(position.x * s, position.y * s), rotation);
        return std::shared_ptr<b2Shape>(box_shape);
    }
};

} //namespace a2d

#endif //A2D_BOX_COLLIDER_H
