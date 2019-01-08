//
// Created by selya on 20.12.2018.
//

#ifndef A2D_LINE_H
#define A2D_LINE_H

#include <a2d/core/drawable.hpp>
#include <a2d/math/vector.hpp>
#include <a2d/graphics/shader.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/resources.hpp>

#include <vector>

namespace a2d {

class Line : public Drawable {
    GLuint vbo;

public:
    struct Vertex {
        Vector2f position;
        Vector4f color;

        Vertex(Vector2f position, Vector4f color) : position(position), color(color) {}
    };

    std::vector<Vertex> vertices;
    pShader shader;

    Line() : shader(Resources::Get<Shader>("line")) {}

    void Initialize() override {
        glGenBuffers(1, &vbo);
    }

    void Draw(SpriteBatch &batch) override {
        shader->Bind();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
        auto p = shader->GetAttribute("position");
        auto color = shader->GetAttribute("color");
        if (p) {
            glVertexAttribPointer(p->location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)nullptr);
            glEnableVertexAttribArray(p->location);
        }
        if (color) {
            glVertexAttribPointer(color->location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(Vector2f));
            glEnableVertexAttribArray(color->location);
        }
        shader->SetUniform("camera_matrix", Engine::GetCamera()->GetMatrix());
        shader->SetUniform("transform_matrix", GetObject2D()->GetTransformMatrix());
        glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
        if (p) glDisableVertexAttribArray(p->location);
        if (color) glDisableVertexAttribArray(color->location);
    }
};

} //namespace a2d

#endif //A2D_LINE_H
