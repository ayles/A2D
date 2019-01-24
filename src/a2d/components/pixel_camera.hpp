//
// Created by selya on 23.11.2018.
//

#ifndef A2D_PIXEL_CAMERA_H
#define A2D_PIXEL_CAMERA_H

#include <a2d/components/camera.hpp>

namespace a2d {

class PixelCamera : public Camera {
public:
    PixelCamera();

    void SetScaleFactor(float scale_factor);
    float GetScaleFactor();

    void Update() override;
    const Matrix4f &GetMatrix() override;

private:
    float scale_factor;
};

} //namespace a2d

#endif //A2D_PIXEL_CAMERA_H
