//
// Created by selya on 06.11.2018.
//

#ifndef A2D_CAMERA_H
#define A2D_CAMERA_H

#include <A2D/core/component.h>
#include <A2D/math.h>

namespace a2d {

DECLARE_SMART_POINTER(Camera)

class Camera : public Component {
    friend class Engine;
    friend class Renderer;
public:
    float size = 5.0f;

    const Matrix4f &GetMatrix() {
        return camera_matrix;
    }

private:
    Camera &SetOrtho2D(float left, float right, float bottom, float top) {
        camera_matrix.SetOrtho2D(left, right, bottom, top);
        return *this;
    }

    Matrix4f camera_matrix;
};

} //namespace a2d

#endif //A2D_CAMERA_H
