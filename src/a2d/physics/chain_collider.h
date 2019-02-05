//
// Created by selya on 05.02.2019.
//

#ifndef A2D_CHAIN_COLLIDER_H
#define A2D_CHAIN_COLLIDER_H

#include <a2d/physics/physics_collider.hpp>

#include <vector>
#include <cmath>

namespace a2d {

class ChainCollider : public PhysicsCollider {
    std::vector<Vector2f> vertices;

#ifndef NDEBUG
    intrusive_ptr<Line> line;
#endif

public:
    Vector2f GetVertex(int index) {
        if (index >= vertices.size()) return Vector2f();
        return vertices[index];
    }

    void SetVertex(const Vector2f &vertex, int index) {
        if (index >= vertices.size()) {
            while (vertices.size() != index) {
                vertices.emplace_back(0, 0);
            }
            vertices.push_back(vertex);
        } else {
            vertices[index] = vertex;
        }
        Reattach();
    }

    void RemoveVertex(int index) {
        if (index >= vertices.size()) return;
        vertices.erase(vertices.begin() + index);
        Reattach();
    }

protected:
    void Initialize() override {
        vertices.emplace_back(0);
        vertices.emplace_back(1);
    }

    std::shared_ptr<b2Shape> CalculateShape(const Vector2f &position, float rotation) override {
        auto chain_shape = new b2ChainShape;
        float s = Physics::GetWorldScale();
        float cos = std::cos(rotation);
        float sin = std::sin(rotation);
        std::vector<b2Vec2> vs;
        vs.reserve(vertices.size());
        for (auto &vertex : vertices) {
            vs.emplace_back(
                    s * (vertex.x * cos - vertex.y * sin) + position.x,
                    s * (vertex.x * sin + vertex.y * cos) + position.y);
        }
        chain_shape->CreateChain(&vs[0], vs.size());
#ifndef NDEBUG
        if (!line) line = GetObject2D()->AddComponent<Line>();
        line->vertices.clear();
        for (auto &vertice : vertices) {
            line->vertices.emplace_back(vertice, Vector4f(0, 1, 0, 1));
        }
        line->Load();
#endif
        return std::shared_ptr<b2Shape>(chain_shape);
    }
};

}

#endif //A2D_CHAIN_COLLIDER_H
