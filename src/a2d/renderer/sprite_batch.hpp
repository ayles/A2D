//
// Created by selya on 24.11.2018.
//

#ifndef A2D_SPRITE_BATCH_H
#define A2D_SPRITE_BATCH_H

#include <a2d/core/ref_counter.hpp>
#include <a2d/core/macro.hpp>
#include <a2d/renderer/texture/texture_region.hpp>
#include <a2d/renderer/shader.hpp>
#include <a2d/math/math.hpp>
#include <a2d/renderer/gl.hpp>
#include <a2d/renderer/material.hpp>

#include <vector>


namespace a2d {

DECLARE_SMART_POINTER(SpriteBatch)

class SpriteBatch : public ref_counter {
    Matrix4f camera_matrix;
    GLuint vbo;
    GLuint indices;
    pMaterial current_material;
    std::vector<float> buffer;
    Vector4f v;

    static const int max_sprites;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(SpriteBatch)
    SpriteBatch();
    ~SpriteBatch() override;

    void SetCameraMatrix(const Matrix4f &camera_matrix);

    const Matrix4f &GetCameraMatrix() const;

    void Begin();
    void End();

    void Draw(const pMaterial &material,
            const Vector2f &uv_lower, const Vector2f &uv_upper,
            const Vector2f &p1, const Vector2f &p2, const Vector2f &p3, const Vector2f &p4,
            const Matrix4f &matrix, const Vector4f &color);

    void Flush();
};

} //namespace a2d

#endif //A2D_SPRITE_BATCH_H
