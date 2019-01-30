//
// Created by selya on 25.11.2018.
//

#include <a2d/graphics/sprite_batch.hpp>
#include <a2d/core/engine.hpp>

namespace a2d {

const int SpriteBatch::max_sprites = 2048;

SpriteBatch::SpriteBatch() : camera_matrix(), current_texture(nullptr),
current_shader(nullptr) {
    ASSERT_MAIN_THREAD
    buffer.reserve(max_sprites * 32);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

    std::vector<unsigned int> indices;
    indices.reserve(max_sprites * 6);
    for (unsigned short i = 0; i < max_sprites; ++i) {
        indices.push_back(i * 4 + 0u);
        indices.push_back(i * 4 + 1u);
        indices.push_back(i * 4 + 2u);
        indices.push_back(i * 4 + 0u);
        indices.push_back(i * 4 + 2u);
        indices.push_back(i * 4 + 3u);
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, max_sprites * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
}

SpriteBatch::~SpriteBatch() {
    ASSERT_MAIN_THREAD
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &indices);
}

void SpriteBatch::SetCameraMatrix(const Matrix4f &camera_matrix) {
    ASSERT_MAIN_THREAD
    this->camera_matrix = camera_matrix;
}

const Matrix4f &SpriteBatch::GetCameraMatrix() const {
    ASSERT_MAIN_THREAD
    return camera_matrix;
}

void SpriteBatch::Draw(const pTextureRegion &texture_region, const pShader &shader,
        const Vector2f &p1, const Vector2f &p2, const Vector2f &p3, const Vector2f &p4,
        const Matrix4f &matrix, const Vector4f &color) {
    ASSERT_MAIN_THREAD
    static Vector2f uv_lb, uv_rt;

    if (!shader) return;
    if (buffer.size() >= max_sprites || (texture_region && (current_texture != texture_region->texture ||
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
    buffer.push_back(v.x);
    buffer.push_back(v.y);
    buffer.push_back(uv_lb.x);
    buffer.push_back(uv_lb.y);
    buffer.push_back(color.x);
    buffer.push_back(color.y);
    buffer.push_back(color.z);
    buffer.push_back(color.w);

    matrix.Transform(p2.x, p2.y, 0, 1, v);
    buffer.push_back(v.x);
    buffer.push_back(v.y);
    buffer.push_back(uv_rt.x);
    buffer.push_back(uv_lb.y);
    buffer.push_back(color.x);
    buffer.push_back(color.y);
    buffer.push_back(color.z);
    buffer.push_back(color.w);

    matrix.Transform(p3.x, p3.y, 0, 1, v);
    buffer.push_back(v.x);
    buffer.push_back(v.y);
    buffer.push_back(uv_rt.x);
    buffer.push_back(uv_rt.y);
    buffer.push_back(color.x);
    buffer.push_back(color.y);
    buffer.push_back(color.z);
    buffer.push_back(color.w);

    matrix.Transform(p4.x, p4.y, 0, 1, v);
    buffer.push_back(v.x);
    buffer.push_back(v.y);
    buffer.push_back(uv_lb.x);
    buffer.push_back(uv_rt.y);
    buffer.push_back(color.x);
    buffer.push_back(color.y);
    buffer.push_back(color.z);
    buffer.push_back(color.w);
}

void SpriteBatch::Flush() {
    ASSERT_MAIN_THREAD
    if (buffer.empty()) return;
    if (!current_shader) {
        buffer.clear();
        return;
    }
    current_shader->Bind();
    if (current_texture) {
        current_texture->Bind(0, filtering, wrapping);
    }

    current_shader->SetUniform("camera_matrix", camera_matrix);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, buffer.size() * sizeof(float), &buffer[0]);
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
    // TODO check on mobile devices
    glDrawElements(GL_TRIANGLES, buffer.size() / 32 * 6, GL_UNSIGNED_INT, nullptr);
    if (p) glDisableVertexAttribArray(p->location);
    if (uv) glDisableVertexAttribArray(uv->location);
    if (color) glDisableVertexAttribArray(color->location);

    buffer.clear();
}

} //namespace a2d