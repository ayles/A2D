//
// Created by selya on 26.01.2019.
//

#ifndef A2D_POLYGON_COLLIDER_H
#define A2D_POLYGON_COLLIDER_H

#include <a2d/physics/physics_collider.hpp>

#include <vector>

namespace a2d {

class PolygonCollider : public PhysicsCollider {
#ifndef NDEBUG
    intrusive_ptr<Line> line;
#endif

    std::vector<Vector2f> vertices;
    b2PolygonShape shape;

public:
    void SetAsBox(float half_x, float half_y) {
        vertices.clear();
        vertices.emplace_back(-half_x, -half_y);
        vertices.emplace_back(half_x, -half_y);
        vertices.emplace_back(half_x, half_y);
        vertices.emplace_back(-half_x, half_y);
        Reattach();
    }

    void SetAsBox(const Vector2f &half_size) {
        SetAsBox(half_size.x, half_size.y);
    }

    void Set(const std::vector<Vector2f> &vertices) {
        this->vertices = vertices;
        Reattach();
    }

    const std::vector<Vector2f> &GetVertices() const {
        return vertices;
    }

protected:
    b2Shape *CalculateShape(b2Body *body) override {
        if (vertices.size() < 3) return nullptr;
#ifndef NDEBUG
        if (!line) line = GetObject2D()->AddComponent<Line>();
        line->vertices.clear();
        for (auto &v : vertices) {
            line->vertices.emplace_back(v, Vector4f(0, 1, 0, 1));
        }
        line->vertices.emplace_back(line->vertices[0]);
        line->Load();
#endif
        float s = Physics::GetWorldScale();
        auto rb = (Rigidbody *)body->GetUserData();
        float cos = std::cos(-rb->GetObject2D()->GetRotation());
        float sin = std::sin(-rb->GetObject2D()->GetRotation());
        Vector2f pos = -rb->GetObject2D()->GetPosition();
        std::vector<b2Vec2> vs;
        vs.reserve(vertices.size());
        for (auto vertex : vertices) {
            auto v = GetObject2D()->GetTransformMatrix().Transform(vertex.x, vertex.y, 0, 1);
            vs.emplace_back(
                    s * (v.x * cos - v.y * sin + pos.x),
                    s * (v.x * sin + v.y * cos + pos.y));
        }
        shape.Set(&vs[0], vs.size());
        return &shape;
    }
};

} //namespace a2d

#endif //A2D_POLYGON_COLLIDER_H
