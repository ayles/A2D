//
// Created by selya on 19.11.2018.
//
#include <a2d/input/input.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/renderer/renderer.hpp>

#include <map>
#include "input.hpp"


namespace a2d {

Input::Touch::Touch() : index(0), position(), state(Touch::TouchState::TOUCH_STATE_RELEASED) {}

Input::Touch::Touch(int index, Vector2f position, Input::Touch::TouchState state) :
                    index(index), position(position), state(state) {}

Input::KeyState Input::GetKeyState(Input::KeyCode key_code) {
    ASSERT_MAIN_THREAD
    return GetKeyInternalState(key_code).state;
}

bool Input::IsKeyJustPressed(Input::KeyCode key_code) {
    ASSERT_MAIN_THREAD
    return GetKeyInternalState(key_code).last_pressed == Engine::GetFrameIndex();
}

bool Input::IsKeyJustReleased(Input::KeyCode key_code) {
    ASSERT_MAIN_THREAD
    return GetKeyInternalState(key_code).last_released == Engine::GetFrameIndex();
}

Vector2f Input::GetScrollDelta() {
    ASSERT_MAIN_THREAD
    return GetInternalScrollDelta();
}

Vector2f Input::GetMousePosition() {
    ASSERT_MAIN_THREAD
    Vector2f pos = GetInternalMousePosition();
    pos.y = Renderer::GetHeight() - pos.y;
    return pos;
}

Input::KeyState Input::GetMouseButtonState(Input::MouseButtonCode button_code) {
    ASSERT_MAIN_THREAD
    return GetMouseButtonInternalState(button_code).state;
}

bool Input::IsMouseButtonJustPressed(Input::MouseButtonCode button_code) {
    ASSERT_MAIN_THREAD
    return GetMouseButtonInternalState(button_code).last_pressed == Engine::GetFrameIndex();
}

bool Input::IsMouseButtonJustReleased(Input::MouseButtonCode button_code) {
    ASSERT_MAIN_THREAD
    return GetMouseButtonInternalState(button_code).last_released == Engine::GetFrameIndex();
}

int Input::GetTouchesCount() {
    ASSERT_MAIN_THREAD
    return GetInternalTouchesCount();
}

Input::Touch Input::GetTouch(int touch_index) {
    ASSERT_MAIN_THREAD
    auto touch_internal_state = GetTouchInternalState(touch_index);

    Touch::TouchState state = Touch::TouchState::TOUCH_STATE_RELEASED;

    if (touch_internal_state.state == Touch::TouchState::TOUCH_STATE_PRESSED) {
        state = (touch_internal_state.last_pressed == Engine::GetFrameIndex()) ?
                Touch::TouchState::TOUCH_STATE_JUST_PRESSED : Touch::TouchState::TOUCH_STATE_PRESSED;
    } else if (touch_internal_state.state == Touch::TouchState::TOUCH_STATE_RELEASED) {
        state = (touch_internal_state.last_released == Engine::GetFrameIndex()) ?
                Touch::TouchState::TOUCH_STATE_JUST_RELEASED : Touch::TouchState::TOUCH_STATE_RELEASED;
    }

    Vector2f pos = touch_internal_state.position;
    pos.y = Renderer::GetHeight() - pos.y;
    return { touch_index, pos, state };
}

bool Input::Initialize() {
#if TARGET_DESKTOP
    glfwSetKeyCallback(Renderer::window, KeyCallback);
    glfwSetCursorPosCallback(Renderer::window, MousePositionCallback);
    glfwSetMouseButtonCallback(Renderer::window, MouseButtonCallback);
    glfwSetScrollCallback(Renderer::window, ScrollCallback);
#endif
    return true;
}

bool Input::Step() {
#if TARGET_DESKTOP
    // For fast input handling
    glfwPollEvents();
#elif TARGET_MOBILE
    while (!GetInternalDelayedTouchesPool().empty()) {
        auto &s = GetInternalDelayedTouchesPool().front();
        auto &touch_internal_state = GetTouchInternalState(s.first);
        auto &button_internal_state = GetMouseButtonInternalState(
                MouseButtonCode::MOUSE_BUTTON_LEFT);

        if (s.second.last_pressed) touch_internal_state.last_pressed = Engine::GetFrameIndex();
        if (s.second.last_released) touch_internal_state.last_released = Engine::GetFrameIndex();
        touch_internal_state.state = s.second.state;
        touch_internal_state.position = s.second.position;

        if (s.first == 0) {
            if (s.second.last_pressed) button_internal_state.last_pressed = touch_internal_state.last_pressed;
            if (s.second.last_released) button_internal_state.last_released = touch_internal_state.last_released;
            switch (s.second.state) {
                case Touch::TouchState::TOUCH_STATE_JUST_PRESSED:
                case Touch::TouchState::TOUCH_STATE_PRESSED:
                    button_internal_state.state = KeyState::KEY_STATE_PRESSED;
                    break;

                case Touch::TouchState::TOUCH_STATE_JUST_RELEASED:
                case Touch::TouchState::TOUCH_STATE_RELEASED:
                    button_internal_state.state = KeyState::KEY_STATE_RELEASED;
                    break;
            }
            GetInternalMousePosition().Set(s.second.position);
        }

        GetInternalDelayedTouchesPool().pop();
    }
#endif
    return true;
}

void Input::Uninitialize() {
#if TARGET_DESKTOP
    glfwSetKeyCallback(Renderer::window, nullptr);
    glfwSetCursorPosCallback(Renderer::window, nullptr);
    glfwSetMouseButtonCallback(Renderer::window, nullptr);
    glfwSetScrollCallback(Renderer::window, nullptr);
#endif
}

#if TARGET_DESKTOP
void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto &key_internal_state = GetKeyInternalState((KeyCode)key);

    switch (action) {
        case GLFW_PRESS:
            key_internal_state.last_pressed = Engine::GetFrameIndex();
            key_internal_state.state = KeyState::KEY_STATE_PRESSED;
            break;

        case GLFW_RELEASE:
            key_internal_state.last_released = Engine::GetFrameIndex();
            key_internal_state.state = KeyState::KEY_STATE_RELEASED;
            break;

        case GLFW_REPEAT:
            // if we lost a PRESS event
            if (key_internal_state.state == KeyState::KEY_STATE_RELEASED) {
                key_internal_state.last_pressed = Engine::GetFrameIndex();
                key_internal_state.state = KeyState::KEY_STATE_PRESSED;
            }
            break;

        default:
            return;
    }
}

void Input::MousePositionCallback(GLFWwindow *window, double x_position, double y_position) {
    GetInternalMousePosition().Set((float)x_position, (float)y_position);
}

void Input::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    auto &button_internal_state = GetMouseButtonInternalState((MouseButtonCode)button);

    switch (action) {
        case GLFW_PRESS:
            button_internal_state.last_pressed = Engine::GetFrameIndex();
            button_internal_state.state = KeyState::KEY_STATE_PRESSED;
            break;

        case GLFW_RELEASE:
            button_internal_state.last_released = Engine::GetFrameIndex();
            button_internal_state.state = KeyState::KEY_STATE_RELEASED;
            break;

        default:
            return;
    }
}

void Input::ScrollCallback(GLFWwindow *window, double x_offset, double y_offset) {
    GetInternalScrollDelta().Set((float)x_offset, (float)y_offset);
}

#elif TARGET_MOBILE

void Input::TouchCallback(int touches_count, int touch_index, TouchEvent touch_event, float touch_x, float touch_y) {
    TouchInternalState touch_state {};

    switch (touch_event) {
        case TouchEvent::TOUCH_BEGAN:
            touch_state.last_pressed = Engine::GetFrameIndex();
            touch_state.state = Touch::TouchState::TOUCH_STATE_PRESSED;
            break;

        case TouchEvent::TOUCH_ENDED:
            touch_state.last_released = Engine::GetFrameIndex();
            touch_state.state = Touch::TouchState::TOUCH_STATE_RELEASED;
            break;

        case TouchEvent::TOUCH_MOVED:
            // if we lost a DOWN event
            if (touch_state.state == Touch::TouchState::TOUCH_STATE_RELEASED) {
                touch_state.last_pressed = Engine::GetFrameIndex();
                touch_state.state = Touch::TouchState::TOUCH_STATE_PRESSED;
            }
            break;

        default:
            return;
    }

    GetInternalDelayedTouchesCount() = touches_count;
    touch_state.position.Set(touch_x, touch_y);
    GetInternalDelayedTouchesPool().emplace(touch_index, touch_state);
}

#endif

int &Input::GetInternalTouchesCount() {
    static int touches_count = 0;
    return touches_count;
}

Input::KeyInternalState &Input::GetKeyInternalState(Input::KeyCode key_code) {
    static std::map<KeyCode, KeyInternalState> keys_states;
    return keys_states[key_code];
}

Input::KeyInternalState &Input::GetMouseButtonInternalState(Input::MouseButtonCode button_code) {
    static std::map<MouseButtonCode , KeyInternalState> buttons_states;
    return buttons_states[button_code];
}

Input::TouchInternalState &Input::GetTouchInternalState(int touch_index) {
    static std::map<int, TouchInternalState> touches_states;
    return touches_states[touch_index];
}

Vector2f &Input::GetInternalMousePosition() {
    static Vector2f mouse_position;
    return mouse_position;
}

Vector2f &Input::GetInternalScrollDelta() {
    static Vector2f scroll_delta;
    return scroll_delta;
}

std::queue<std::pair<int, Input::TouchInternalState>> &Input::GetInternalDelayedTouchesPool() {
    static std::queue<std::pair<int, TouchInternalState>> q;
    return q;
}

int &Input::GetInternalDelayedTouchesCount() {
    static int count = 0;
    return count;
}


} //namespace a2d
