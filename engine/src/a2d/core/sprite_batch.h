//
// Created by selya on 24.11.2018.
//

#ifndef A2D_SPRITE_BATCH_H
#define A2D_SPRITE_BATCH_H

#include <a2d/core/ref_counter.h>
#include <a2d/core/macro.h>
#include <a2d/graphics/texture_region.h>
#include <a2d/graphics/shader.h>
#include <a2d/math/math.h>
#include <a2d/core/components/camera.h>

#include <a2d/core/gl.h>

namespace a2d {

DECLARE_SMART_POINTER(SpriteBatch)

class SpriteBatch : public ref_counter {
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

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(SpriteBatch)
    SpriteBatch();
    ~SpriteBatch() override;

    void SetCameraMatrix(const Matrix4f &camera_matrix);

    const Matrix4f &GetCameraMatrix() const;

    void Draw(const pTextureRegion &texture_region, const pShader &shader,
            const Vector2f &p1, const Vector2f &p2, const Vector2f &p3, const Vector2f &p4,
            const Matrix4f &matrix, const Vector4f &color);

    void Flush();
};

} //namespace a2d

#endif //A2D_SPRITE_BATCH_H
