//
// Created by selya on 06.11.2018.
//

#ifndef A2D_CAMERA_H
#define A2D_CAMERA_H

#include <a2d/core/component.h>
#include <a2d/math.h>

namespace a2d {

DECLARE_SMART_POINTER(Camera)

class Camera : public Component {
    friend class Engine;
    friend class Renderer;

public:
    float GetHeight();
    float GetWidth();
    float GetAspectRatio();

    void SetHeight(float height);

    Vector2f ScreenToWorld(const Vector2f &screen);

    const Matrix4f &GetMatrix();

private:
    float height = 5.0f;
    Matrix4f camera_matrix;
    Matrix4f camera_transformed_matrix;

    Camera &SetOrtho2D(float left, float right, float bottom, float top);
};

} //namespace a2d

#endif //A2D_CAMERA_H
