//
// Created by selya on 19.11.2018.
//

#ifndef A2D_INPUT_HPP
#define A2D_INPUT_HPP

#include <a2d/renderer/gl.hpp>
#include <a2d/math/vector.hpp>

#include <queue>

namespace a2d {

class Input {
    friend class NativeBridge;

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
        KEY_STATE_RELEASED = 0,
        KEY_STATE_PRESSED = 1
    };

    enum TouchEvent {
        TOUCH_BEGAN,
        TOUCH_MOVED,
        TOUCH_ENDED
    };

    struct Touch {
        enum TouchState {
            TOUCH_STATE_RELEASED = 0,
            TOUCH_STATE_PRESSED = 1,
            TOUCH_STATE_JUST_RELEASED = 2,
            TOUCH_STATE_JUST_PRESSED = 3
        };

        int index;
        Vector2f position;
        TouchState state;

        Touch();
        Touch(int index, Vector2f position, TouchState state);
    };



    /**
     * Returns key current state either PRESSED or RELEASED.
     *
     * @param key_code key code
     * @return key state
     */
    static KeyState GetKeyState(KeyCode key_code);

    /**
     * Shows whether key was pressed in the current frame.
     *
     * @param key_code key code
     * @return true if key was pressed during current frame, false otherwise
     */
    static bool IsKeyJustPressed(KeyCode key_code);

    /**
     * Shows whether key was released in the current frame.
     *
     * @param key_code key code
     * @return true if key was released during current frame, false otherwise
     */
    static bool IsKeyJustReleased(KeyCode key_code);

    /**
     * Returns scroll delta during this frame.
     *
     * Simple mouse wheel provides only y-delta (vertical).
     *
     * @return scroll delta
     */
    static Vector2f GetScrollDelta();

    /**
     * Returns mouse position relative to bottom-left screen corner.
     *
     * On mobile platforms it is just touch 0 position.
     *
     * @return mouse position
     */
    static Vector2f GetMousePosition();

    /**
     * Returns mouse button current state either PRESSED or RELEASED.
     *
     * On mobile platforms it is same as touch 0 state.
     *
     * @param button_code mouse button code
     * @return mouse button state
     */
    static KeyState GetMouseButtonState(MouseButtonCode button_code);

    /**
     * Shows whether mouse button was pressed in the current frame.
     *
     * On mobile platforms it is same as touch 0 state.
     *
     * @param button_code mouse button code
     * @return true if button was pressed during current frame, false otherwise
     */
    static bool IsMouseButtonJustPressed(MouseButtonCode button_code);

    /**
     * Shows whether mouse button was released in the current frame.
     *
     * On mobile platforms it is same as touch 0 state.
     *
     * @param button_code mouse button code
     * @return true if button was released during current frame, false otherwise
     */
    static bool IsMouseButtonJustReleased(MouseButtonCode button_code);

    /**
     * Returns number of currently active touches.
     *
     * @return number of currently active touches
     */
    static int GetTouchesCount();

    /**
     * Returns touch by its index.
     *
     * @param touch_index touch index
     * @return touch
     */
    static Touch GetTouch(int touch_index);

private:
    static bool Initialize();
    static bool Step();
    static void Uninitialize();

#if TARGET_DESKTOP
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void MousePositionCallback(GLFWwindow *window, double x_position, double y_position);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow *window, double x_offset, double y_offset);
#elif TARGET_MOBILE
    static void TouchCallback(int touches_count, int touch_index, TouchEvent touch_event, float touch_x, float touch_y);
#endif

    struct KeyInternalState {
        unsigned long long last_pressed = 0;
        unsigned long long last_released = 0;
        KeyState state = KeyState::KEY_STATE_RELEASED;
    };

    struct TouchInternalState {
        unsigned long long last_pressed = 0;
        unsigned long long last_released = 0;
        Touch::TouchState state = Touch::TouchState::TOUCH_STATE_RELEASED;
        Vector2f position = Vector2f();
    };

    static int &GetInternalTouchesCount();
    static KeyInternalState &GetKeyInternalState(KeyCode key_code);
    static KeyInternalState &GetMouseButtonInternalState(MouseButtonCode button_code);
    static TouchInternalState &GetTouchInternalState(int touch_index);
    static Vector2f &GetInternalMousePosition();
    static Vector2f &GetInternalScrollDelta();
    static std::queue<std::pair<int, TouchInternalState>> &GetInternalDelayedTouchesPool();
    static int &GetInternalDelayedTouchesCount();
};

} //namespace a2d

#endif //A2D_INPUT_HPP
