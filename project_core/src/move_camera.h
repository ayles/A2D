//
// Created by selya on 27.11.2018.
//

#ifndef DESKTOP_MOVE_CAMERA_H
#define DESKTOP_MOVE_CAMERA_H

#include <a2d/core/component.h>
#include <a2d/math/math.h>
#include <a2d/input/input.h>
#include <a2d/core/object2d.h>
#include <a2d/core/components/camera.h>

class MoveCamera : public a2d::Component {
    a2d::Vector2f direction = a2d::Vector2f(0.0f, 0.0f);
    float speed = 200.0f;

    void Update() override {
        direction = 0;

        if (a2d::Input::GetKeyState(a2d::Input::KeyCode::KEY_W)) direction.y += 1;
        if (a2d::Input::GetKeyState(a2d::Input::KeyCode::KEY_S)) direction.y -= 1;
        if (a2d::Input::GetKeyState(a2d::Input::KeyCode::KEY_A)) direction.x -= 1;
        if (a2d::Input::GetKeyState(a2d::Input::KeyCode::KEY_D)) direction.x += 1;

        if (direction.LengthSquared() > 0)
            direction.Normalize();

        GetObject2D()->position += direction * a2d::Engine::GetDeltaTime() * speed;
    }
};

#endif //DESKTOP_MOVE_CAMERA_H
