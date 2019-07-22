#ifndef SATURN_INPUT_HPP_
#define SATURN_INPUT_HPP_

#include <functional>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Core/Application.hpp"
#include "Utility/IDGenerator.hpp"

namespace Saturn {

class Application;

enum class Key {
    Space = GLFW_KEY_SPACE,
    Apostrophe = GLFW_KEY_APOSTROPHE,
    Comma = GLFW_KEY_COMMA,
    Minus = GLFW_KEY_MINUS,
    Period = GLFW_KEY_PERIOD,
    Slash = GLFW_KEY_SLASH,
    Key0 = GLFW_KEY_0,
    Key1 = GLFW_KEY_1,
    Key2 = GLFW_KEY_2,
    Key3 = GLFW_KEY_3,
    Key4 = GLFW_KEY_4,
    Key5 = GLFW_KEY_5,
    Key6 = GLFW_KEY_6,
    Key7 = GLFW_KEY_7,
    Key8 = GLFW_KEY_8,
    Key9 = GLFW_KEY_9,
    Semicolon = GLFW_KEY_SEMICOLON,
    Equal = GLFW_KEY_EQUAL,
    A = GLFW_KEY_A,
    B = GLFW_KEY_B,
    C = GLFW_KEY_C,
    D = GLFW_KEY_D,
    E = GLFW_KEY_E,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    I = GLFW_KEY_I,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    M = GLFW_KEY_M,
    N = GLFW_KEY_N,
    O = GLFW_KEY_O,
    P = GLFW_KEY_P,
    Q = GLFW_KEY_Q,
    R = GLFW_KEY_R,
    S = GLFW_KEY_S,
    T = GLFW_KEY_T,
    U = GLFW_KEY_U,
    V = GLFW_KEY_V,
    W = GLFW_KEY_W,
    X = GLFW_KEY_X,
    Y = GLFW_KEY_Y,
    Z = GLFW_KEY_Z,
    LeftBracket = GLFW_KEY_LEFT_BRACKET,
    Backslash = GLFW_KEY_BACKSLASH,
    RightBracket = GLFW_KEY_RIGHT_BRACKET,
    GraveAccent = GLFW_KEY_GRAVE_ACCENT,
    World1 = GLFW_KEY_WORLD_1,
    World2 = GLFW_KEY_WORLD_2,
    Escape = GLFW_KEY_ESCAPE,
    Enter = GLFW_KEY_ENTER,
    Tab = GLFW_KEY_TAB,
    Backspace = GLFW_KEY_BACKSPACE,
    Insert = GLFW_KEY_INSERT,
    Delete = GLFW_KEY_DELETE,
    Right = GLFW_KEY_RIGHT,
    Left = GLFW_KEY_LEFT,
    Down = GLFW_KEY_DOWN,
    Up = GLFW_KEY_UP,
    PageUp = GLFW_KEY_PAGE_UP,
    PageDown = GLFW_KEY_PAGE_DOWN,
    Home = GLFW_KEY_HOME,
    End = GLFW_KEY_END,
    CapsLock = GLFW_KEY_CAPS_LOCK,
    ScrollLock = GLFW_KEY_SCROLL_LOCK,
    NumLock = GLFW_KEY_NUM_LOCK,
    PrintScreen = GLFW_KEY_PRINT_SCREEN,
    Pause = GLFW_KEY_PAUSE,
    F1 = GLFW_KEY_F1,
    F2 = GLFW_KEY_F2,
    F3 = GLFW_KEY_F3,
    F4 = GLFW_KEY_F4,
    F5 = GLFW_KEY_F5,
    F6 = GLFW_KEY_F6,
    F7 = GLFW_KEY_F7,
    F8 = GLFW_KEY_F8,
    F9 = GLFW_KEY_F9,
    F10 = GLFW_KEY_F10,
    F11 = GLFW_KEY_F11,
    F12 = GLFW_KEY_F12,
    F13 = GLFW_KEY_F13,
    F14 = GLFW_KEY_F14,
    F15 = GLFW_KEY_F15,
    F16 = GLFW_KEY_F16,
    F17 = GLFW_KEY_F17,
    F18 = GLFW_KEY_F18,
    F19 = GLFW_KEY_F19,
    F20 = GLFW_KEY_F20,
    F21 = GLFW_KEY_F21,
    F22 = GLFW_KEY_F22,
    F23 = GLFW_KEY_F23,
    F24 = GLFW_KEY_F24,
    F25 = GLFW_KEY_F25,
    Kp0 = GLFW_KEY_KP_0,
    Kp1 = GLFW_KEY_KP_1,
    Kp2 = GLFW_KEY_KP_2,
    Kp3 = GLFW_KEY_KP_3,
    Kp4 = GLFW_KEY_KP_4,
    Kp5 = GLFW_KEY_KP_5,
    Kp6 = GLFW_KEY_KP_6,
    Kp7 = GLFW_KEY_KP_7,
    Kp8 = GLFW_KEY_KP_8,
    Kp9 = GLFW_KEY_KP_9,
    KpDecimal = GLFW_KEY_KP_DECIMAL,
    KpDivide = GLFW_KEY_KP_DIVIDE,
    KpMultiply = GLFW_KEY_KP_MULTIPLY,
    KpSubtract = GLFW_KEY_KP_SUBTRACT,
    KpAdd = GLFW_KEY_KP_ADD,
    KpEnter = GLFW_KEY_KP_ENTER,
    KpEqual = GLFW_KEY_KP_EQUAL,
    LeftShift = GLFW_KEY_LEFT_SHIFT,
    LeftControl = GLFW_KEY_LEFT_CONTROL,
    LeftAlt = GLFW_KEY_LEFT_ALT,
    LeftSuper = GLFW_KEY_LEFT_SUPER,
    RightShift = GLFW_KEY_RIGHT_SHIFT,
    RightControl = GLFW_KEY_RIGHT_CONTROL,
    RightAlt = GLFW_KEY_RIGHT_ALT,
    RightSuper = GLFW_KEY_RIGHT_SUPER,
    Menu = GLFW_KEY_MENU,
    GamepadA = GLFW_GAMEPAD_BUTTON_A,
    GamepadB = GLFW_GAMEPAD_BUTTON_B,
    GamepadX = GLFW_GAMEPAD_BUTTON_X,
    GamepadY = GLFW_GAMEPAD_BUTTON_Y,
    GamepadLeftBumper = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
    GamepadRightBumper = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
    GamepadBack = GLFW_GAMEPAD_BUTTON_BACK,
    GamepadStart = GLFW_GAMEPAD_BUTTON_START,
    GamepadGuide = GLFW_GAMEPAD_BUTTON_GUIDE,
    GamepadLeftThumb = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
    GamepadRightThumb = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
    GamepadDPadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP,
    GamepadDPadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
    GamepadDPadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
    GamepadDPadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
    GamepadLeftXAxis = GLFW_GAMEPAD_AXIS_LEFT_X,
    GamepadLeftYAxis = GLFW_GAMEPAD_AXIS_LEFT_Y,
    GamepadRightXAxis = GLFW_GAMEPAD_AXIS_RIGHT_X,
    GamepadRightYAxis = GLFW_GAMEPAD_AXIS_RIGHT_Y,
    GamepadLeftTriggerAxis = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
    GamepadRightTriggerAxis = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
    Unknown = GLFW_KEY_UNKNOWN
};

// Aliases for PS4 controllers
static constexpr inline Key KeyGamepadCross = Key::GamepadA;
static constexpr inline Key KeyGamepadCircle = Key::GamepadB;
static constexpr inline Key KeyGamepadSquare = Key::GamepadX;
static constexpr inline Key KeyGamepadTriangle = Key::GamepadY;

enum class MouseButton {
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Right = GLFW_MOUSE_BUTTON_RIGHT,
    Middle = GLFW_MOUSE_BUTTON_MIDDLE,
    Button1 = GLFW_MOUSE_BUTTON_1,
    Button2 = GLFW_MOUSE_BUTTON_2,
    Button3 = GLFW_MOUSE_BUTTON_3,
    Button4 = GLFW_MOUSE_BUTTON_4,
    Button5 = GLFW_MOUSE_BUTTON_5,
    Button6 = GLFW_MOUSE_BUTTON_6,
    Button7 = GLFW_MOUSE_BUTTON_7,
    Button8 = GLFW_MOUSE_BUTTON_8
};
enum class JoystickId {
    Joystick1 = GLFW_JOYSTICK_1,
    Joystick2 = GLFW_JOYSTICK_2,
    Joystick3 = GLFW_JOYSTICK_3,
    Joystick4 = GLFW_JOYSTICK_4,
    Joystick5 = GLFW_JOYSTICK_5,
    Joystick6 = GLFW_JOYSTICK_6,
    Joystick7 = GLFW_JOYSTICK_7,
    Joystick8 = GLFW_JOYSTICK_8,
    Joystick9 = GLFW_JOYSTICK_9,
    Joystick10 = GLFW_JOYSTICK_10,
    Joystick11 = GLFW_JOYSTICK_11,
    Joystick12 = GLFW_JOYSTICK_12,
    Joystick13 = GLFW_JOYSTICK_13,
    Joystick14 = GLFW_JOYSTICK_14,
    Joystick15 = GLFW_JOYSTICK_15,
    Joystick16 = GLFW_JOYSTICK_16
};

enum class KeyAction { Press = GLFW_PRESS, Release = GLFW_RELEASE };

struct KeyState {
    float raw_value = 0.0f;
    float value = 0.0f;
    bool down = false;
    bool has_changed = false;
};

struct MouseState {
    float xoffset = 0.0f;
    float yoffset = 0.0f;
    float wheel = 0.0f;
    bool has_changed = false;
};

class InputOld {
public:
    using CallbackT = std::function<void()>;
    using KeyT = int;

    struct MouseData {
        float xpos;
        float ypos;

        float xscroll;
        float yscroll;
    };

    // /brief Initializes the Input system. Stores a pointer to the program
    // passed in
    static void initialize(Application& program);

    template<typename Callback>
    static void bind(KeyT key, Callback&& func) {
        keybinds[key] = std::forward<Callback>(func);
    }

    static bool key_pressed(KeyT key);
    static void update();

    static void tick_end();

    static void enable_mouse_capture();

    static MouseData mouse();
    static MouseData previous_mouse();

private:
    static void mouse_callback([[maybe_unused]] GLFWwindow* window,
                               double xpos,
                               double ypos);
    static void scroll_callback([[maybe_unused]] GLFWwindow* window,
                                double xoffset,
                                double yoffset);

    static std::unordered_map<KeyT, CallbackT> keybinds;
    static Application* app;

    static MouseData current;
    static MouseData previous;
};

class RawInput {
public:
    static KeyState& get_key(Key key);
    static std::unordered_map<Key, KeyState>& get_all_keys();
    static KeyState& get_mouse_button(MouseButton button);
    static std::unordered_map<MouseButton, KeyState>& get_all_mouse_buttons();
    static MouseState& get_mouse();

private:
    static std::unordered_map<Key, KeyState> keys;
    static std::unordered_map<MouseButton, KeyState> mouse_buttons;
    static MouseState mouse;
};

struct Axis {
    // The axis name
    std::string name;
    // The axis ID
    std::size_t id;
    // Smoothed value of the axis
    float value = 0.0f;
    // Raw value of the axis
    float raw_value = 0.0f;
};

struct AxisMapping {
    // The name of the axis this mapping refers to
    std::string name;
    // The key this axis mapping works with
    Key key = Key::Unknown;
    // Value to multiply the raw axis value with
    float scale = 1.0f;
    // How fast the axis accumulates, in units/s
    float sensitivity = 1.0f;
    // The smoothed value of the axis
    float value = 0.0f;
    // The raw value of the axis
    float raw_value = 0.0f;
};

struct ActionBinding {
    using callback_t = std::function<void()>;
    // The key that triggers this action
    Key key;
    // The state the key needs to have for the action to take place
    KeyAction when;
    // The callback function to call
    callback_t callback;
};

class AxisManager {
public:
    static void add_axis(std::string_view name);
    static void add_axis_mapping(AxisMapping const& mapping);

    static void update_axis_values();

    static std::vector<Axis>& get_axes();
    static std::size_t get_axis_id(std::string const& name);

private:
    // Stores all axes
    static std::vector<Axis> axes;
    // Stores all axis mappings
    static std::vector<AxisMapping> mappings;
    // Maps an axis name to an axis ID
    static std::unordered_map<std::string, std::size_t> name_id_map;
};

class InputEventManager {
public:
    struct KeyEvent {
        Key key;
        KeyAction action;
        float value;
    };

    struct MouseEvent {
        float mouse_x;
        float mouse_y;
        float wheel;
    };

    struct MouseClickEvent {
        MouseButton button;
        KeyAction action;
        float value;
    };

    static void init(Application& program);

    static void process_events();

private:
    static void keyboard_callback(
        GLFWwindow* win, int key, int scancode, int action, int mods);
    static void
    mouse_position_callback(GLFWwindow* win, double xpos, double ypos);
    static void
    mouse_button_callback(GLFWwindow* win, int button, int action, int mods);
    static void
    scroll_callback(GLFWwindow* win, double xoffset, double yoffset);

    static void process_keyboard_events();
    static void process_mouse_events();

    struct Events {
        std::queue<KeyEvent> key_events;
        std::queue<MouseEvent> mouse_events;
        std::queue<MouseClickEvent> mouse_click_events;
    };

    static Events events;
    static Application* app;
};

class JoystickInputManager {
public:
    static void find_present_joysticks();
    static void joystick_connection_callback(int id, int event);
    static void update_key_data();

    struct JoystickData {
        bool connected;
        bool is_gamepad;
        std::string name;
        // This may be different from the joystick name. Only present if
        // is_gamepad is true
        std::string gamepad_name;
    };

private:
    static std::unordered_map<JoystickId, JoystickData> present_joysticks;
};

class ActionBindingManager {
public:
    static void add_action(ActionBinding const& action);
    static void trigger_if_event(InputEventManager::KeyEvent const& event);

private:
    static std::vector<ActionBinding> actions;
};

class Input {
public:
    static float get_axis(std::string const& name);
    static float get_axis_raw(std::string const& name);

private:
};

} // namespace Saturn

#endif
