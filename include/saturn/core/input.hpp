#ifndef SATURN_INPUT_HPP_
#define SATURN_INPUT_HPP_

#include <functional>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>
#include <stl/vector.hpp>

#include <mimas/mimas.h>

#include <phobos/forward.hpp>

namespace saturn {

enum class Key {
    Space = MIMAS_KEY_SPACE,
    Apostrophe = MIMAS_KEY_APOSTROPHE,
    Comma = MIMAS_KEY_COMMA,
    Minus = MIMAS_KEY_MINUS,
    Slash = MIMAS_KEY_SLASH,
    Key0 = MIMAS_KEY_0,
    Key1 = MIMAS_KEY_1,
    Key2 = MIMAS_KEY_2,
    Key3 = MIMAS_KEY_3,
    Key4 = MIMAS_KEY_4,
    Key5 = MIMAS_KEY_5,
    Key6 = MIMAS_KEY_6,
    Key7 = MIMAS_KEY_7,
    Key8 = MIMAS_KEY_8,
    Key9 = MIMAS_KEY_9,
    Semicolon = MIMAS_KEY_SEMICOLON,
    A = MIMAS_KEY_A,
    B = MIMAS_KEY_B,
    C = MIMAS_KEY_C,
    D = MIMAS_KEY_D,
    E = MIMAS_KEY_E,
    F = MIMAS_KEY_F,
    G = MIMAS_KEY_G,
    H = MIMAS_KEY_H,
    I = MIMAS_KEY_I,
    J = MIMAS_KEY_J,
    K = MIMAS_KEY_K,
    L = MIMAS_KEY_L,
    M = MIMAS_KEY_M,
    N = MIMAS_KEY_N,
    O = MIMAS_KEY_O,
    P = MIMAS_KEY_P,
    Q = MIMAS_KEY_Q,
    R = MIMAS_KEY_R,
    S = MIMAS_KEY_S,
    T = MIMAS_KEY_T,
    U = MIMAS_KEY_U,
    V = MIMAS_KEY_V,
    W = MIMAS_KEY_W,
    X = MIMAS_KEY_X,
    Y = MIMAS_KEY_Y,
    Z = MIMAS_KEY_Z,
    LeftBracket = MIMAS_KEY_LEFT_BRACKET,
    RightBracket = MIMAS_KEY_RIGHT_BRACKET,
    Escape = MIMAS_KEY_ESCAPE,
    Enter = MIMAS_KEY_ENTER,
    Tab = MIMAS_KEY_TAB,
    Backspace = MIMAS_KEY_BACKSPACE,
    Insert = MIMAS_KEY_INSERT,
    Delete = MIMAS_KEY_DELETE,
    Right = MIMAS_KEY_RIGHT,
    Left = MIMAS_KEY_LEFT,
    Down = MIMAS_KEY_DOWN,
    Up = MIMAS_KEY_UP,
    PageUp = MIMAS_KEY_PAGE_UP,
    PageDown = MIMAS_KEY_PAGE_DOWN,
    Home = MIMAS_KEY_HOME,
    End = MIMAS_KEY_END,
    CapsLock = MIMAS_KEY_CAPS_LOCK,
    ScrollLock = MIMAS_KEY_SCROLL_LOCK,
    PrintScreen = MIMAS_KEY_PRINT_SCREEN,
    Pause = MIMAS_KEY_PAUSE,
    F1 = MIMAS_KEY_F1,
    F2 = MIMAS_KEY_F2,
    F3 = MIMAS_KEY_F3,
    F4 = MIMAS_KEY_F4,
    F5 = MIMAS_KEY_F5,
    F6 = MIMAS_KEY_F6,
    F7 = MIMAS_KEY_F7,
    F8 = MIMAS_KEY_F8,
    F9 = MIMAS_KEY_F9,
    Kp0 = MIMAS_KEY_NUMPAD_0,
    Kp1 = MIMAS_KEY_NUMPAD_1,
    Kp2 = MIMAS_KEY_NUMPAD_2,
    Kp3 = MIMAS_KEY_NUMPAD_3,
    Kp4 = MIMAS_KEY_NUMPAD_4,
    Kp5 = MIMAS_KEY_NUMPAD_5,
    Kp6 = MIMAS_KEY_NUMPAD_6,
    Kp7 = MIMAS_KEY_NUMPAD_7,
    Kp8 = MIMAS_KEY_NUMPAD_8,
    Kp9 = MIMAS_KEY_NUMPAD_9,
    KpDecimal = MIMAS_KEY_NUMPAD_DECIMAL,
    KpDivide = MIMAS_KEY_NUMPAD_DIVIDE,
    KpMultiply = MIMAS_KEY_NUMPAD_MULTIPLY,
    KpSubtract = MIMAS_KEY_NUMPAD_SUBTRACT,
    KpAdd = MIMAS_KEY_NUMPAD_ADD,
    KpEnter = MIMAS_KEY_NUMPAD_ENTER,
    LeftShift = MIMAS_KEY_LEFT_SHIFT,
    LeftControl = MIMAS_KEY_LEFT_CONTROL,
    LeftAlt = MIMAS_KEY_LEFT_ALT,
    RightShift = MIMAS_KEY_RIGHT_SHIFT,
    RightControl = MIMAS_KEY_RIGHT_CONTROL,
    RightAlt = MIMAS_KEY_RIGHT_ALT,
    Unknown = MIMAS_KEY_UNKNOWN
};

enum class MouseButton {
    Left = MIMAS_MOUSE_LEFT_BUTTON,
    Right = MIMAS_MOUSE_RIGHT_BUTTON,
    Middle = MIMAS_MOUSE_MIDDLE_BUTTON
};
/*
enum class JoystickId {
    Joystick1 = MIMAS_JOYSTICK_1,
    Joystick2 = MIMAS_JOYSTICK_2,
    Joystick3 = MIMAS_JOYSTICK_3,
    Joystick4 = MIMAS_JOYSTICK_4,
    Joystick5 = MIMAS_JOYSTICK_5,
    Joystick6 = MIMAS_JOYSTICK_6,
    Joystick7 = MIMAS_JOYSTICK_7,
    Joystick8 = MIMAS_JOYSTICK_8,
    Joystick9 = MIMAS_JOYSTICK_9,
    Joystick10 = MIMAS_JOYSTICK_10,
    Joystick11 = MIMAS_JOYSTICK_11,
    Joystick12 = MIMAS_JOYSTICK_12,
    Joystick13 = MIMAS_JOYSTICK_13,
    Joystick14 = MIMAS_JOYSTICK_14,
    Joystick15 = MIMAS_JOYSTICK_15,
    Joystick16 = MIMAS_JOYSTICK_16
};
*/


enum class KeyAction { Press = MIMAS_KEY_PRESS, Release = MIMAS_KEY_RELEASE };

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
    stl::size_t id;
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

    static stl::vector<Axis>& get_axes();
    static stl::size_t get_axis_id(std::string const& name);

private:
    // Stores all axes
    static stl::vector<Axis> axes;
    // Stores all axis mappings
    static stl::vector<AxisMapping> mappings;
    // Maps an axis name to an axis ID
    static std::unordered_map<std::string, stl::size_t> name_id_map;
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

    static void init(ph::WindowContext& window_ctx);

    static void process_events();

private:
    static void keyboard_callback(Mimas_Window* win, Mimas_Key key, Mimas_Key_Action action, void*);
    static void mouse_position_callback(Mimas_Window* win, mimas_i32 xpos, mimas_i32 ypos, void*);
    static void mouse_button_callback(Mimas_Window* win, Mimas_Key button, Mimas_Mouse_Button_Action action, void*);
    static void scroll_callback(Mimas_Window* win, double xoffset, double yoffset);

    static void process_keyboard_events();
    static void process_mouse_events();

    struct Events {
        std::queue<KeyEvent> key_events;
        std::queue<MouseEvent> mouse_events;
        std::queue<MouseClickEvent> mouse_click_events;
    };

    static Events events;
    static ph::WindowContext* window_ctx;
};

/*
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
*/

class ActionBindingManager {
public:
    static void add_action(ActionBinding const& action);
    static void trigger_if_event(InputEventManager::KeyEvent const& event);

private:
    static stl::vector<ActionBinding> actions;
};

class Input {
public:
    static float get_axis(std::string const& name);
    static float get_axis_raw(std::string const& name);

    static void initialize(ph::WindowContext& window_ctx);
    static void load_config_file(std::string const& path);
    static void set_mouse_capture(bool capture);

private:
    static ph::WindowContext* window_ctx;
};

} // namespace saturn

#endif
