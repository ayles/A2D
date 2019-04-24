//
// Created by selya on 05.02.2019.
//

#ifndef A2D_CHAIN_COLLIDER_HPP
#define A2D_CHAIN_COLLIDER_HPP

#include <a2d/physics/physics_collider.hpp>

#include <vector>
#include <cmath>

namespace a2d {

class ChainCollider : public PhysicsCollider {
#ifndef NDEBUG
    intrusive_ptr<Line> line;
#endif

    std::vector<Vector2f> vertices;
    Vector2f prev_vertex;
    Vector2f next_vertex;
    bool has_prev_vertex = false;
    bool has_next_vertex = false;
    b2ChainShape shape;

public:
    void CreateLoop(const std::vector<Vector2f> &vertices) {
        this->vertices = vertices;
        this->vertices.emplace_back(vertices[0]);
        prev_vertex = this->vertices[this->vertices.size() - 2];
        next_vertex = this->vertices[1];
        has_prev_vertex = true;
        has_next_vertex = true;
        Reattach();
    }

    void CreateChain(const std::vector<Vector2f> &vertices) {
        this->vertices = vertices;
        has_prev_vertex = false;
        has_next_vertex = false;
        prev_vertex.Zero();
        next_vertex.Zero();
        Reattach();
    }

    void SetNextVertex(const Vector2f &vertex) {
        shape.SetNextVertex(b2Vec2(vertex.x, vertex.y));
        Reattach();
    }

    void SetPrevVertex(const Vector2f &vertex) {
        shape.SetPrevVertex(b2Vec2(vertex.x, vertex.y));
        Reattach();
    }

    void SetHasNextVertex(bool has) {
        shape.m_hasNextVertex = has;
        Reattach();
    }

    void SetHasPrevVertex(bool has) {
        shape.m_hasPrevVertex = has;
        Reattach();
    }

    bool GetHasNextVertex() {
        return shape.m_hasNextVertex;
    }

    bool GetHasPrevVertex() {
        return shape.m_hasPrevVertex;
    }

protected:
    b2Shape *CalculateShape(b2Body *body) override {
        if (vertices.size() < 2) return nullptr;
#ifndef NDEBUG
        if (!line) {
            auto o = Object2D::Create();
            o->Attach(GetObject2D());
            line = o->AddComponent<Line>();
        }
        line->vertices.clear();
        for (auto &vertex : vertices) {
            line->vertices.emplace_back(vertex, Vector4f(0, 1, 0, 1));
        }
        line->Load();
#endif
        float s = Physics::GetWorldScale();
        auto rb = (Rigidbody *)body->GetUserData();
        float cos = std::cos(-rb->GetObject2D()->GetRotation());
        float sin = std::sin(-rb->GetObject2D()->GetRotation());
        Vector2f pos = -rb->GetObject2D()->GetPosition();
        std::vector<b2Vec2> vs;
        vs.reserve(vertices.size());
        for (auto &vertex : vertices) {
            auto v = GetObject2D()->GetTransformMatrix().Transform(vertex.x, vertex.y, 0, 1);
            vs.emplace_back(
                    s * (v.x * cos - v.y * sin + pos.x),
                    s * (v.x * sin + v.y * cos + pos.y));
        }
        shape.Clear();
        shape.CreateChain(&vs[0], vs.size());
        auto v = GetObject2D()->GetTransformMatrix().Transform(next_vertex.x, next_vertex.y, 0, 1);
        shape.m_nextVertex = b2Vec2(
                s * (v.x * cos - v.y * sin + pos.x),
                s * (v.x * sin + v.y * cos + pos.y));
        v = GetObject2D()->GetTransformMatrix().Transform(prev_vertex.x, prev_vertex.y, 0, 1);
        shape.m_prevVertex = b2Vec2(
                s * (v.x * cos - v.y * sin + pos.x),
                s * (v.x * sin + v.y * cos + pos.y));
        shape.m_hasNextVertex = has_next_vertex;
        shape.m_hasPrevVertex = has_prev_vertex;
        return &shape;
    }
};

}

#endif //A2D_CHAIN_COLLIDER_HPP
