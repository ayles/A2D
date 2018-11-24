//
// Created by selya on 24.11.2018.
//

#ifndef A2D_SPRITE_BATCH_H
#define A2D_SPRITE_BATCH_H

#include <a2d/core/ref_counter.h>
#include <a2d/core/macro.h>
#include <a2d/core/texture.h>
#include <a2d/core/shader.h>
#include <a2d/math.h>
#include <a2d/core/engine.h>
#include <a2d/core/components/camera.h>

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#elif TARGET_DESKTOP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

namespace a2d {

DECLARE_SMART_POINTER(SpriteBatch)

class SpriteBatch : public ref_counter {
public:
    SpriteBatch() : buffer_size(0), buffer_capacity(8192), buffer(new float[buffer_capacity]),
        current_shader(nullptr), current_texture(nullptr), camera_matrix() {
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

        unsigned short indices[12288];
        for (unsigned short i = 0; i < 2048; ++i) {
            indices[i * 6 + 0] = static_cast<unsigned short>(i * 4 + 0);
            indices[i * 6 + 1] = static_cast<unsigned short>(i * 4 + 1);
            indices[i * 6 + 2] = static_cast<unsigned short>(i * 4 + 2);
            indices[i * 6 + 3] = static_cast<unsigned short>(i * 4 + 0);
            indices[i * 6 + 4] = static_cast<unsigned short>(i * 4 + 2);
            indices[i * 6 + 5] = static_cast<unsigned short>(i * 4 + 3);
        }

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 12288, indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, buffer_capacity * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    }

    void SetCameraMatrix(const Matrix4f &camera_matrix) {
        this->camera_matrix = camera_matrix;
    }

    const Matrix4f &GetCameraMatrix() {
        return camera_matrix;
    }

    void Draw(const pTextureRegion &texture_region, const pShader &shader, const Matrix4f &matrix, const Vector4f &color) {
        if (!texture_region || !shader) return;
        if (buffer_size >= buffer_capacity || current_texture != texture_region->texture || current_shader != shader ||
            texture_region->filtering != filtering || texture_region->wrapping != wrapping) {
            Flush();
            current_texture = texture_region->texture;
            current_shader = shader;
            filtering = texture_region->filtering;
            wrapping = texture_region->wrapping;
        }

        float half_width = texture_region->ratio * 0.5f;
        float half_height = 0.5f;

        matrix.Transform(-half_width, -half_height, 0, 1, v);
        buffer[buffer_size++] = v.x;
        buffer[buffer_size++] = v.y;
        buffer[buffer_size++] = texture_region->uv_lb.x;
        buffer[buffer_size++] = texture_region->uv_lb.y;

        matrix.Transform(half_width, -half_height, 0, 1, v);
        buffer[buffer_size++] = v.x;
        buffer[buffer_size++] = v.y;
        buffer[buffer_size++] = texture_region->uv_rt.x;
        buffer[buffer_size++] = texture_region->uv_lb.y;

        matrix.Transform(half_width, half_height, 0, 1, v);
        buffer[buffer_size++] = v.x;
        buffer[buffer_size++] = v.y;
        buffer[buffer_size++] = texture_region->uv_rt.x;
        buffer[buffer_size++] = texture_region->uv_rt.y;

        matrix.Transform(-half_width, half_height, 0, 1, v);
        buffer[buffer_size++] = v.x;
        buffer[buffer_size++] = v.y;
        buffer[buffer_size++] = texture_region->uv_lb.x;
        buffer[buffer_size++] = texture_region->uv_rt.y;


    }

    void Flush() {
        if (!buffer_size) return;
        if (!current_shader) { buffer_size = 0; return; }
        current_shader->Bind();
        if (!current_texture) { buffer_size = 0; return; }
        current_texture->Bind(0, filtering, wrapping);

        current_shader->SetUniform("camera_matrix", camera_matrix);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_size * sizeof(float), buffer);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)nullptr);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(sizeof(float) * 2));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawElements(GL_TRIANGLES, buffer_size / 8 * 3, GL_UNSIGNED_SHORT, nullptr);

        buffer_size = 0;
    }

    ~SpriteBatch() {
        delete [] buffer;
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &indices);
    }

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(SpriteBatch)

private:
    Matrix4f camera_matrix;
    GLuint vbo;
    GLuint indices;
    Texture::Filtering filtering;
    Texture::Wrapping wrapping;
    pTexture current_texture;
    pShader current_shader;
    int buffer_size;
    int buffer_capacity;
    float *buffer;
    Vector4f v;
};

} //namespace a2d

#endif //A2D_SPRITE_BATCH_H
