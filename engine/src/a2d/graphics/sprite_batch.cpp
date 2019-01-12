//
// Created by selya on 25.11.2018.
//

#include <a2d/graphics/sprite_batch.hpp>
#include <a2d/core/engine.hpp>

namespace a2d {

SpriteBatch::SpriteBatch() : camera_matrix(), current_texture(nullptr),
current_shader(nullptr), buffer_size(0), buffer_capacity(32768), buffer(new float[buffer_capacity]) {
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

    auto indices = new unsigned short[12288];
    for (unsigned short i = 0; i < 2048; ++i) {
        indices[i * 6 + 0] = static_cast<unsigned short>(i * 4 + 0);
        indices[i * 6 + 1] = static_cast<unsigned short>(i * 4 + 1);
        indices[i * 6 + 2] = static_cast<unsigned short>(i * 4 + 2);
        indices[i * 6 + 3] = static_cast<unsigned short>(i * 4 + 0);
        indices[i * 6 + 4] = static_cast<unsigned short>(i * 4 + 2);
        indices[i * 6 + 5] = static_cast<unsigned short>(i * 4 + 3);
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 12288, indices, GL_STATIC_DRAW);
    delete [] indices;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, buffer_capacity * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
}

SpriteBatch::~SpriteBatch() {
    delete [] buffer;
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &indices);
}

void SpriteBatch::SetCameraMatrix(const Matrix4f &camera_matrix) {
    this->camera_matrix = camera_matrix;
}

const Matrix4f &SpriteBatch::GetCameraMatrix() const {
    return camera_matrix;
}

void
SpriteBatch::Draw(const pTextureRegion &texture_region, const pShader &shader, const Vector2f &p1, const Vector2f &p2,
                  const Vector2f &p3, const Vector2f &p4, const Matrix4f &matrix, const Vector4f &color) {
    static Vector2f uv_lb, uv_rt;

    if (!shader) return;
    if (buffer_size >= buffer_capacity || (texture_region && (current_texture != texture_region->texture ||
        texture_region->filtering != filtering || texture_region->wrapping != wrapping)) || current_shader != shader) {
        Flush();
        if (texture_region) {
            current_texture = texture_region->texture;
            filtering = texture_region->filtering;
            wrapping = texture_region->wrapping;
        }
        current_shader = shader;
    }

    if (texture_region) {
        texture_region->RecalculateUV();
        uv_lb = texture_region->uv_lb;
        uv_rt = texture_region->uv_rt;
    }

    matrix.Transform(p1.x, p1.y, 0, 1, v);
    buffer[buffer_size++] = v.x;
    buffer[buffer_size++] = v.y;
    buffer[buffer_size++] = uv_lb.x;
    buffer[buffer_size++] = uv_lb.y;
    buffer[buffer_size++] = color.x;
    buffer[buffer_size++] = color.y;
    buffer[buffer_size++] = color.z;
    buffer[buffer_size++] = color.w;

    matrix.Transform(p2.x, p2.y, 0, 1, v);
    buffer[buffer_size++] = v.x;
    buffer[buffer_size++] = v.y;
    buffer[buffer_size++] = uv_rt.x;
    buffer[buffer_size++] = uv_lb.y;
    buffer[buffer_size++] = color.x;
    buffer[buffer_size++] = color.y;
    buffer[buffer_size++] = color.z;
    buffer[buffer_size++] = color.w;

    matrix.Transform(p3.x, p3.y, 0, 1, v);
    buffer[buffer_size++] = v.x;
    buffer[buffer_size++] = v.y;
    buffer[buffer_size++] = uv_rt.x;
    buffer[buffer_size++] = uv_rt.y;
    buffer[buffer_size++] = color.x;
    buffer[buffer_size++] = color.y;
    buffer[buffer_size++] = color.z;
    buffer[buffer_size++] = color.w;

    matrix.Transform(p4.x, p4.y, 0, 1, v);
    buffer[buffer_size++] = v.x;
    buffer[buffer_size++] = v.y;
    buffer[buffer_size++] = uv_lb.x;
    buffer[buffer_size++] = uv_rt.y;
    buffer[buffer_size++] = color.x;
    buffer[buffer_size++] = color.y;
    buffer[buffer_size++] = color.z;
    buffer[buffer_size++] = color.w;
}

void SpriteBatch::Flush() {
    if (!buffer_size) return;
    if (!current_shader) { buffer_size = 0; return; }
    current_shader->Bind();
    if (current_texture) {
        current_texture->Bind(0, filtering, wrapping);
    }

    current_shader->SetUniform("camera_matrix", camera_matrix);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_size * sizeof(float), buffer);
    auto p = current_shader->GetAttribute("position");
    auto uv = current_shader->GetAttribute("uv");
    auto color = current_shader->GetAttribute("color");
    if (p) {
        glVertexAttribPointer(p->location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)nullptr);
        glEnableVertexAttribArray(p->location);
    }
    if (uv) {
        glVertexAttribPointer(uv->location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 2));
        glEnableVertexAttribArray(uv->location);
    }
    if (color) {
        glVertexAttribPointer(color->location, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 4));
        glEnableVertexAttribArray(color->location);
    }
    glDrawElements(GL_TRIANGLES, buffer_size / 32 * 6, GL_UNSIGNED_SHORT, nullptr);
    if (p) glDisableVertexAttribArray(p->location);
    if (uv) glDisableVertexAttribArray(uv->location);
    if (color) glDisableVertexAttribArray(color->location);

    buffer_size = 0;
}

} //namespace a2d
