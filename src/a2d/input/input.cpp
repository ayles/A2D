//
// Created by selya on 19.11.2018.
//
#include <a2d/input/input.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/renderer/renderer.hpp>

#include <map>

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
    auto &touch_internal_state = GetTouchInternalState(touch_index);
    auto &button_internal_state = GetMouseButtonInternalState(MouseButtonCode::MOUSE_BUTTON_LEFT);

    switch (touch_event) {
        case TouchEvent::TOUCH_BEGAN:
            touch_internal_state.last_pressed = Engine::GetFrameIndex();
            touch_internal_state.state = Touch::TouchState::TOUCH_STATE_PRESSED;
            if (touch_index == 0) {
                button_internal_state.last_pressed = Engine::GetFrameIndex();
                button_internal_state.state = KeyState::KEY_STATE_PRESSED;
            }
            break;

        case TouchEvent::TOUCH_ENDED:
            touch_internal_state.last_released = Engine::GetFrameIndex();
            touch_internal_state.state = Touch::TouchState::TOUCH_STATE_RELEASED;
            if (touch_index == 0) {
                button_internal_state.last_released = Engine::GetFrameIndex();
                button_internal_state.state = KeyState::KEY_STATE_RELEASED;
            }
            break;

        case TouchEvent::TOUCH_MOVED:
            // if we lost a DOWN event
            if (touch_internal_state.state == Touch::TouchState::TOUCH_STATE_RELEASED) {
                touch_internal_state.last_pressed = Engine::GetFrameIndex();
                touch_internal_state.state = Touch::TouchState::TOUCH_STATE_PRESSED;
                if (touch_index == 0) {
                    button_internal_state.last_released = Engine::GetFrameIndex();
                    button_internal_state.state = KeyState::KEY_STATE_PRESSED;
                }
            }
            break;

        default:
            return;
    }

    GetInternalTouchesCount() = touches_count;
    touch_internal_state.position.Set(touch_x, touch_y);

    if (touch_index == 0) {
        GetInternalMousePosition().Set(touch_x, touch_y);
    }
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


} //namespace a2d
