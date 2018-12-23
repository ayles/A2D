//
// Created by selya on 20.12.2018.
//

#ifndef A2D_LINE_H
#define A2D_LINE_H

#include <a2d/core/drawable.h>
#include <a2d/math/vector.h>
#include <a2d/core/shader.h>
#include <a2d/core/object2d.h>

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

    Line() : shader(Shader::GetShader("line")) {}

    void Initialize() override {
        glGenBuffers(1, &vbo);
    }

    void Draw(SpriteBatch &batch) override {
        shader->Bind();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(Vector2f));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(2);
        shader->SetUniform("camera_matrix", Engine::GetCamera()->GetMatrix());
        shader->SetUniform("transform_matrix", GetObject2D()->GetTransformMatrix());
        glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(2);
    }
};

} //namespace a2d

#endif //A2D_LINE_H
