//
// Created by selya on 06.11.2018.
//

#ifndef A2D_CAMERA_HPP
#define A2D_CAMERA_HPP

#include <a2d/core/component.hpp>
#include <a2d/math/math.hpp>

namespace a2d {

class Framebuffer;
class SpriteBatch;

class Camera : public Component {
    friend class Engine;
    friend class Renderer;

public:
    float GetHeight();
    float GetWidth();
    float GetAspectRatio();
    const Vector4f &GetClearColor();
    intrusive_ptr<Framebuffer> GetFramebuffer();

    void SetHeight(float height);
    void SetClearColor(float r, float g, float b, float a);
    void SetClearColor(const Vector4f &color);
    void SetFramebuffer(const intrusive_ptr<Framebuffer> &framebuffer);

    Vector2f ScreenToWorld(const Vector2f &screen);

    const Matrix4f &GetMatrix();
    const Matrix4f &GetTransformedMatrix();

    void Render();

protected:
    float height = 5.0f;
    Matrix4f camera_matrix;
    Matrix4f camera_transformed_matrix;
    Vector4f clear_color = Vector4f(0, 0, 0, 1);
    intrusive_ptr<Framebuffer> framebuffer;
    intrusive_ptr<SpriteBatch> sprite_batch;

    void SetOrtho2D(float left, float right, float bottom, float top);
};

} //namespace a2d

#endif //A2D_CAMERA_HPP
