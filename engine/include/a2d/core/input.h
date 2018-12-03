//
// Created by selya on 19.11.2018.
//

#ifndef A2D_INPUT_NEW_H
#define A2D_INPUT_NEW_H

#include <a2d/core/renderer.h>

#ifdef TARGET_DESKTOP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#elif TARGET_ANDROID
#include <android/input.h>
#endif

namespace a2d {

class Input {
    friend class NativeConnector;

public:
    enum KeyCode {
        KEY_UNKNOWN         = -1,

        KEY_SPACE           = 32,
        KEY_APOSTROPHE      = 39, /* ' */
        KEY_COMMA           = 44, /* , */
        KEY_MINUS           = 45, /* - */
        KEY_PERIOD          = 46, /* . */
        KEY_SLASH           = 47, /* / */
        KEY_0               = 48,
        KEY_1               = 49,
        KEY_2               = 50,
        KEY_3               = 51,
        KEY_4               = 52,
        KEY_5               = 53,
        KEY_6               = 54,
        KEY_7               = 55,
        KEY_8               = 56,
        KEY_9               = 57,
        KEY_SEMICOLON       = 59, /* ; */
        KEY_EQUAL           = 61, /* = */
        KEY_A               = 65,
        KEY_B               = 66,
        KEY_C               = 67,
        KEY_D               = 68,
        KEY_E               = 69,
        KEY_F               = 70,
        KEY_G               = 71,
        KEY_H               = 72,
        KEY_I               = 73,
        KEY_J               = 74,
        KEY_K               = 75,
        KEY_L               = 76,
        KEY_M               = 77,
        KEY_N               = 78,
        KEY_O               = 79,
        KEY_P               = 80,
        KEY_Q               = 81,
        KEY_R               = 82,
        KEY_S               = 83,
        KEY_T               = 84,
        KEY_U               = 85,
        KEY_V               = 86,
        KEY_W               = 87,
        KEY_X               = 88,
        KEY_Y               = 89,
        KEY_Z               = 90,
        KEY_LEFT_BRACKET    = 91, /* [ */
        KEY_BACKSLASH       = 92, /* \ */
        KEY_RIGHT_BRACKET   = 93, /* ] */
        KEY_GRAVE_ACCENT    = 96, /* ` */
        KEY_WORLD_1         = 161, /* non-US #1 */
        KEY_WORLD_2         = 162, /* non-US #2 */

        KEY_ESCAPE          = 256,
        KEY_ENTER           = 257,
        KEY_TAB             = 258,
        KEY_BACKSPACE       = 259,
        KEY_INSERT          = 260,
        KEY_DELETE          = 261,
        KEY_RIGHT           = 262,
        KEY_LEFT            = 263,
        KEY_DOWN            = 264,
        KEY_UP              = 265,
        KEY_PAGE_UP         = 266,
        KEY_PAGE_DOWN       = 267,
        KEY_HOME            = 268,
        KEY_END             = 269,
        KEY_CAPS_LOCK       = 280,
        KEY_SCROLL_LOCK     = 281,
        KEY_NUM_LOCK        = 282,
        KEY_PRINT_SCREEN    = 283,
        KEY_PAUSE           = 284,
        KEY_F1              = 290,
        KEY_F2              = 291,
        KEY_F3              = 292,
        KEY_F4              = 293,
        KEY_F5              = 294,
        KEY_F6              = 295,
        KEY_F7              = 296,
        KEY_F8              = 297,
        KEY_F9              = 298,
        KEY_F10             = 299,
        KEY_F11             = 300,
        KEY_F12             = 301,
        KEY_F13             = 302,
        KEY_F14             = 303,
        KEY_F15             = 304,
        KEY_F16             = 305,
        KEY_F17             = 306,
        KEY_F18             = 307,
        KEY_F19             = 308,
        KEY_F20             = 309,
        KEY_F21             = 310,
        KEY_F22             = 311,
        KEY_F23             = 312,
        KEY_F24             = 313,
        KEY_F25             = 314,
        KEY_KP_0            = 320,
        KEY_KP_1            = 321,
        KEY_KP_2            = 322,
        KEY_KP_3            = 323,
        KEY_KP_4            = 324,
        KEY_KP_5            = 325,
        KEY_KP_6            = 326,
        KEY_KP_7            = 327,
        KEY_KP_8            = 328,
        KEY_KP_9            = 329,
        KEY_KP_DECIMAL      = 330,
        KEY_KP_DIVIDE       = 331,
        KEY_KP_MULTIPLY     = 332,
        KEY_KP_SUBTRACT     = 333,
        KEY_KP_ADD          = 334,
        KEY_KP_ENTER        = 335,
        KEY_KP_EQUAL        = 336,
        KEY_LEFT_SHIFT      = 340,
        KEY_LEFT_CONTROL    = 341,
        KEY_LEFT_ALT        = 342,
        KEY_LEFT_SUPER      = 343,
        KEY_RIGHT_SHIFT     = 344,
        KEY_RIGHT_CONTROL   = 345,
        KEY_RIGHT_ALT       = 346,
        KEY_RIGHT_SUPER     = 347,
        KEY_MENU            = 348,
    };

    enum MouseButtonCode {
        MOUSE_BUTTON_1      = 0,
        MOUSE_BUTTON_2      = 1,
        MOUSE_BUTTON_3      = 2,
        MOUSE_BUTTON_4      = 3,
        MOUSE_BUTTON_5      = 4,
        MOUSE_BUTTON_6      = 5,
        MOUSE_BUTTON_7      = 6,
        MOUSE_BUTTON_8      = 7,
        MOUSE_BUTTON_LAST   = MOUSE_BUTTON_8,
        MOUSE_BUTTON_LEFT   = MOUSE_BUTTON_1,
        MOUSE_BUTTON_RIGHT  = MOUSE_BUTTON_2,
        MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3
    };

    enum KeyState {
        RELEASED = 0,
        PRESSED = 1
    };

    struct Touch {
        enum TouchState {
            RELEASED = 0,
            PRESSED = 1,
            JUST_RELEASED = 2,
            JUST_PRESSED = 3
        };

        int index = 0;
        Vector2f position = Vector2f();
        TouchState state = RELEASED;

        Touch(int index, Vector2f position, TouchState state) : index(index), position(position), state(state) {}
    };



    // Returns key current state either PRESSED or RELEASED
    static KeyState GetKeyState(KeyCode key_code) {
        return GetKeyInternalState(key_code).state;
    }

    // Returns true if key was pressed during current frame
    static bool IsKeyJustPressed(KeyCode key_code) {
        return GetKeyInternalState(key_code).last_pressed == Engine::GetFrameIndex();
    }

    // Returns true if key was released during current frame
    static bool IsKeyJustReleased(KeyCode key_code) {
        return GetKeyInternalState(key_code).last_released == Engine::GetFrameIndex();
    }

    // Returns mouse position relative to top-left screen corner
    static Vector2f GetMousePosition() {
        return GetInternalMousePosition();
    }

    // Returns mouse button current state either PRESSED or RELEASED
    static KeyState GetMouseButtonState(MouseButtonCode button_code) {
        return GetMouseButtonInternalState(button_code).state;
    }

    // Returns true if mouse button was pressed during current frame
    static bool IsMouseButtonJustPressed(MouseButtonCode button_code) {
        return GetMouseButtonInternalState(button_code).last_pressed == Engine::GetFrameIndex();
    }

    // Returns true if mouse button was released during current frame
    static bool IsMouseButtonJustReleased(MouseButtonCode button_code) {
        return GetMouseButtonInternalState(button_code).last_released == Engine::GetFrameIndex();
    }

    // Returns number of currently active touches
    static int GetTouchesCount() {
        return GetInternalTouchesCount();
    }

    // Get Touch by its index (starting from 0)
    static Touch GetTouch(int touch_index) {
        auto touch_internal_state = GetTouchInternalState(touch_index);

        Touch::TouchState state = Touch::TouchState::RELEASED;

        if (touch_internal_state.state == Touch::TouchState::PRESSED) {
            state = (touch_internal_state.last_pressed == Engine::GetFrameIndex()) ?
                    Touch::TouchState::JUST_PRESSED : Touch::TouchState::PRESSED;
        } else if (touch_internal_state.state == Touch::TouchState::RELEASED) {
            state = (touch_internal_state.last_released == Engine::GetFrameIndex()) ?
                    Touch::TouchState::JUST_RELEASED : Touch::TouchState::RELEASED;
        }

        return { touch_index, touch_internal_state.position, state };
    }

private:
    static bool Initialize() {
#ifdef TARGET_DESKTOP
        glfwSetKeyCallback(Renderer::window, KeyCallback);
        glfwSetCursorPosCallback(Renderer::window, MousePositionCallback);
        glfwSetMouseButtonCallback(Renderer::window, MouseButtonCallback);
#endif
        return true;
    }

    static void Uninitialize() {
#ifdef TARGET_DESKTOP
        glfwSetKeyCallback(Renderer::window, nullptr);
        glfwSetCursorPosCallback(Renderer::window, nullptr);
        glfwSetMouseButtonCallback(Renderer::window, nullptr);
#endif
    }

#ifdef TARGET_DESKTOP
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto &key_internal_state = GetKeyInternalState((KeyCode)key);

        switch (action) {
            case GLFW_PRESS:
                key_internal_state.last_pressed = Engine::GetFrameIndex();
                key_internal_state.state = KeyState::PRESSED;
                break;

            case GLFW_RELEASE:
                key_internal_state.last_released = Engine::GetFrameIndex();
                key_internal_state.state = KeyState::RELEASED;
                break;

            case GLFW_REPEAT:
                // if we loosed a PRESS event
                if (key_internal_state.state == KeyState::RELEASED) {
                    key_internal_state.last_pressed = Engine::GetFrameIndex();
                    key_internal_state.state = KeyState::PRESSED;
                }
                break;

            default:
                return;
        }
    }

    static void MousePositionCallback(GLFWwindow *window, double x_position, double y_position) {
        GetInternalMousePosition().Set((float)x_position, (float)y_position);
    }

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto &button_internal_state = GetMouseButtonInternalState((MouseButtonCode)button);

        switch (action) {
            case GLFW_PRESS:
                button_internal_state.last_pressed = Engine::GetFrameIndex();
                button_internal_state.state = KeyState::PRESSED;
                break;

            case GLFW_RELEASE:
                button_internal_state.last_released = Engine::GetFrameIndex();
                button_internal_state.state = KeyState::RELEASED;
                break;

            default:
                return;
        }
    }



#elif TARGET_ANDROID
    static void TouchCallback(int touches_count, int touch_index, int touch_action, float touch_x, float touch_y) {
        auto &touch_internal_state = GetTouchInternalState(touch_index);
        auto &button_internal_state = GetMouseButtonInternalState(MouseButtonCode::MOUSE_BUTTON_LEFT);

        switch (touch_action) {
            case AMOTION_EVENT_ACTION_DOWN:
                touch_internal_state.last_pressed = Engine::GetFrameIndex();
                touch_internal_state.state = Touch::TouchState::PRESSED;
                if (touch_index == 0) {
                    button_internal_state.last_pressed = Engine::GetFrameIndex();
                    button_internal_state.state = KeyState::PRESSED;
                }
                break;

            case AMOTION_EVENT_ACTION_UP:
                touch_internal_state.last_released = Engine::GetFrameIndex();
                touch_internal_state.state = Touch::TouchState::RELEASED;
                if (touch_index == 0) {
                    button_internal_state.last_released = Engine::GetFrameIndex();
                    button_internal_state.state = KeyState::RELEASED;
                }
                break;

            case AMOTION_EVENT_ACTION_MOVE:
                // if we loosed a DOWN event
                if (touch_internal_state.state == Touch::TouchState::RELEASED) {
                    touch_internal_state.last_pressed = Engine::GetFrameIndex();
                    touch_internal_state.state = Touch::TouchState::PRESSED;
                    if (touch_index == 0) {
                        button_internal_state.last_released = Engine::GetFrameIndex();
                        button_internal_state.state = KeyState::RELEASED;
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

    struct KeyInternalState {
        unsigned long long last_pressed = 0;
        unsigned long long last_released = 0;
        KeyState state = KeyState::RELEASED;
    };

    struct TouchInternalState {
        unsigned long long last_pressed = 0;
        unsigned long long last_released = 0;
        Touch::TouchState state = Touch::TouchState::RELEASED;
        Vector2f position = Vector2f();
    };

    static int &GetInternalTouchesCount() {
        static int touches_count = 0;
        return touches_count;
    }

    static KeyInternalState &GetKeyInternalState(KeyCode key_code) {
        static std::map<KeyCode, KeyInternalState> keys_states;
        return keys_states[key_code];
    }

    static KeyInternalState &GetMouseButtonInternalState(MouseButtonCode button_code) {
        static std::map<MouseButtonCode , KeyInternalState> buttons_states;
        return buttons_states[button_code];
    }

    static TouchInternalState &GetTouchInternalState(int touch_index) {
        static std::map<int, TouchInternalState> touches_states;
        return touches_states[touch_index];
    }

    static Vector2f &GetInternalMousePosition() {
        static Vector2f mouse_position;
        return mouse_position;
    }
};

} //namespace a2d

#endif //A2D_INPUT_NEW_H
