#include "Subsystems/Input/Input.hpp"

#include "Core/Application.hpp"
#include "Subsystems/Time/Time.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp> // #TODO This is for glm::lerp(), check if this doesn't cause issues

namespace Saturn {

std::unordered_map<InputOld::KeyT, InputOld::CallbackT> InputOld::keybinds;
Application* InputOld::app = nullptr;

InputOld::MouseData InputOld::current = {0.0f, 0.0f, 0.0f, 0.0f};
InputOld::MouseData InputOld::previous = {0.0f, 0.0f, 0.0f, 0.0f};

void InputOld::initialize(Application& program) {
    InputEventManager::init(program);

    app = &program;
    // center mouse
    current = previous =
        MouseData{program.size().x / 2.0f, program.size().y / 2.0f, 0.0f, 0.0f};

    // Add axes here temporarily
    AxisManager::add_axis("Horizontal");
    AxisManager::add_axis("Vertical");
    AxisManager::add_axis("Up");
    AxisManager::add_axis("MouseHorizontal");
    AxisManager::add_axis("MouseVertical");
    AxisMapping front_mapping;
    front_mapping.key = Key::W;
    front_mapping.name = "Vertical";
    AxisManager::add_axis_mapping(front_mapping);
    AxisMapping back_mapping;
    back_mapping.key = Key::S;
    back_mapping.name = "Vertical";
    back_mapping.sensitivity = -1.0f;
    AxisManager::add_axis_mapping(back_mapping);
    AxisMapping left_mapping;
    left_mapping.key = Key::A;
    left_mapping.name = "Horizontal";
    left_mapping.sensitivity = -1.0f;
    AxisManager::add_axis_mapping(left_mapping);
    AxisMapping right_mapping;
    right_mapping.key = Key::D;
    right_mapping.name = "Horizontal";
    AxisManager::add_axis_mapping(right_mapping);
    AxisMapping up_mapping;
    up_mapping.key = Key::Space;
    up_mapping.name = "Up";
    AxisManager::add_axis_mapping(up_mapping);
    AxisMapping down_mapping;
    down_mapping.key = Key::LeftShift;
    down_mapping.name = "Up";
    down_mapping.sensitivity = -1.0f;
    AxisManager::add_axis_mapping(down_mapping);

    // Vertical axis has to be inverted
    AxisMapping cf{"Vertical", Key::GamepadRightYAxis, 1.0f, -1.0f};
    AxisMapping cl{"Horizontal", Key::GamepadRightXAxis};

    AxisMapping gamepad_look_hor{"MouseHorizontal", Key::GamepadLeftXAxis,
                                 2.5f};
    AxisMapping gamepad_look_ver{
        "MouseVertical",
        Key::GamepadLeftYAxis,
        2.5f, -1.0f
    };

    AxisManager::add_axis_mapping(cf);
    AxisManager::add_axis_mapping(cl);
    AxisManager::add_axis_mapping(gamepad_look_hor);
    AxisManager::add_axis_mapping(gamepad_look_ver);
}

void InputOld::update() {
    InputEventManager::process_events();
    for (auto& [k, cb] : keybinds) {
        if (glfwGetKey(app->window(), k) == GLFW_PRESS) {
            // Call the callback function the user specified
            cb();
        }
    }
}

void InputOld::tick_end() {
    previous = current;
    current.xscroll = 0.0f;
    current.yscroll = 0.0f;
}

bool InputOld::key_pressed(KeyT key) {
    return glfwGetKey(app->window(), key) == GLFW_PRESS;
}

void InputOld::enable_mouse_capture() {
    glfwSetInputMode(app->window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

InputOld::MouseData InputOld::mouse() { return current; }
InputOld::MouseData InputOld::previous_mouse() { return previous; }

void InputOld::mouse_callback([[maybe_unused]] GLFWwindow* window,
                              double xpos,
                              double ypos) {
    // prevent the camera from making a 'jump' when entering the window for the
    // first time (on startup)
    static bool first = true;

    previous.xpos = current.xpos;
    previous.ypos = current.ypos;
    current.xpos = static_cast<float>(xpos);
    current.ypos = static_cast<float>(ypos);

    if (first) {
        previous = current;
        first = false;
    }
}

void InputOld::scroll_callback([[maybe_unused]] GLFWwindow* window,
                               double xoffset,
                               double yoffset) {

    previous.xscroll = current.xscroll;
    previous.yscroll = current.yscroll;

    current.xscroll = static_cast<float>(xoffset);
    current.yscroll = static_cast<float>(yoffset);
}

static bool is_gamepad_key(Key key) {
    return static_cast<int>(key) >= GLFW_GAMEPAD_BUTTON_A &&
           static_cast<int>(key) <= GLFW_GAMEPAD_BUTTON_LAST;
}

static bool is_gamepad_axis(Key key) {
    return static_cast<int>(key) >= GLFW_GAMEPAD_AXIS_LEFT_X &&
           static_cast<int>(key) <= GLFW_GAMEPAD_AXIS_LAST;
}

// RawInput class

std::unordered_map<Key, KeyState> RawInput::keys;
std::unordered_map<MouseButton, KeyState> RawInput::mouse_buttons;
MouseState RawInput::mouse;

KeyState& RawInput::get_key(Key key) { return keys[key]; }
KeyState& RawInput::get_mouse_button(MouseButton button) {
    return mouse_buttons[button];
}

std::unordered_map<Key, KeyState>& RawInput::get_all_keys() { return keys; }
std::unordered_map<MouseButton, KeyState>& RawInput::get_all_mouse_buttons() {
    return mouse_buttons;
}

MouseState& RawInput::get_mouse() { return mouse; }

// AxisManager class

std::vector<Axis> AxisManager::axes;
std::vector<AxisMapping> AxisManager::mappings;
std::unordered_map<std::string, std::size_t> AxisManager::name_id_map;

void AxisManager::add_axis(std::string_view name) {
    Axis axis;
    axis.name = name;
    axis.id = IDGenerator<Axis>::next();
    name_id_map[axis.name] = axis.id;
    axes.push_back(axis);
}

void AxisManager::add_axis_mapping(AxisMapping const& mapping) {
    assert(mapping.key != Key::Unknown);
    mappings.push_back(mapping);
}

void AxisManager::update_axis_values() {
    auto& keys = RawInput::get_all_keys();
    // Update all axis mappings
    for (auto& mapping : mappings) {
        // Look up the axis this mapping refers to
        auto id = name_id_map[mapping.name];
        if (auto it =
                std::find_if(axes.begin(), axes.end(),
                             [id](Axis const& axis) { return id == axis.id; });
            it != axes.end()) {

            KeyState state;
            if (!is_gamepad_key(mapping.key)) {
                state = keys[mapping.key];
            } else {
                // #TODO For now, we only support a single joystick. This means
                // we just store the joystick key values inside our normal key
                // map
                state = keys[mapping.key];
            }

            if (is_gamepad_axis(mapping.key)) {
                // use raw value for gamepad axes
                mapping.raw_value = state.raw_value;
                mapping.value = glm::sign(mapping.sensitivity) * mapping.scale *
                                state.raw_value;
            } else {
                // Smooth value
                mapping.raw_value = state.raw_value;

                // First, get the raw value scaled
                float scaled_raw = glm::sign(mapping.sensitivity) *
                                   mapping.scale * mapping.raw_value;
                // #Verify
                mapping.raw_value = scaled_raw;

                // #TODO: Snapping
                float delta = glm::abs(mapping.sensitivity) * Time::deltaTime;
                mapping.value = glm::lerp(mapping.value, scaled_raw, delta);
            }
        } else {
            // No axis found with requested name
            LogSystem::write(LogSystem::Severity::Warning,
                             "Invalid axis mapping: No axis exists with name " +
                                 mapping.name);
        }
    }
    // Update all axes
    for (auto& axis : axes) {
        // Reset axis
        axis.raw_value = 0.0f;
        axis.value = 0.0f;
        // Go over all mappings and update the axis value if the mapping refers
        // to this axis
        for (auto& mapping : mappings) {
            if (name_id_map.at(mapping.name) == axis.id) {
                axis.raw_value += mapping.scale * mapping.raw_value;
                axis.value += mapping.value;
            }
        }
    }
}

std::vector<Axis>& AxisManager::get_axes() { return axes; }

std::size_t AxisManager::get_axis_id(std::string const& name) {
    auto it = name_id_map.find(name);
    if (it != name_id_map.end()) {
        return it->second;
    } else {
        LogSystem::write(LogSystem::Severity::Warning,
                         "Invalid axis name: No axis exists with name " + name);
        return -1;
    }
}

// InputEventManager class

// Define static variables
InputEventManager::Events InputEventManager::events;
Application* InputEventManager::app;

void InputEventManager::init(Application& program) {
    glfwSetKeyCallback(program.window(), &InputEventManager::keyboard_callback);
    glfwSetCursorPosCallback(program.window(),
                             &InputEventManager::mouse_position_callback);
    glfwSetMouseButtonCallback(program.window(),
                               &InputEventManager::mouse_button_callback);
    glfwSetScrollCallback(program.window(),
                          &InputEventManager::scroll_callback);
    glfwSetJoystickCallback(JoystickInputManager::joystick_connection_callback);
    JoystickInputManager::find_present_joysticks();
    app = &program;
}

void InputEventManager::keyboard_callback(
    GLFWwindow* win, int raw_key, int scancode, int raw_action, int mods) {

    // We do not want to make holding a key fire a new event every tick
    if (raw_action != GLFW_REPEAT) {
        auto key = static_cast<Key>(raw_key);
        // Since GLFW_PRESS is 1, and GLFW_RELEASE is 0, we can use these values
        // as our axis values
        auto value = static_cast<float>(raw_action);
        auto action = static_cast<KeyAction>(raw_action);
        events.key_events.push(KeyEvent{key, action, value});
    }
}

void InputEventManager::mouse_position_callback(GLFWwindow* win,
                                                double xpos,
                                                double ypos) {
    static float last_x = app->size().x / 2.0f;
    static float last_y = app->size().y / 2.0f;

    auto x = static_cast<float>(xpos);
    auto y = static_cast<float>(ypos);

    float xoffset = x - last_x;
    // Invert y offset
    float yoffset = last_y - y;

    last_x = x;
    last_y = y;

    // Add the event
    events.mouse_events.push(MouseEvent{xoffset, yoffset, 0.0f});
}

void InputEventManager::scroll_callback(GLFWwindow* win,
                                        double xoffset,
                                        double yoffset) {
    events.mouse_events.push(
        MouseEvent{0.0f, 0.0f, static_cast<float>(yoffset)});
}

void InputEventManager::mouse_button_callback(GLFWwindow* win,
                                              int button,
                                              int action,
                                              int mods) {
    if (action != GLFW_REPEAT) {
        auto value = static_cast<float>(action);
        events.mouse_click_events.push(
            MouseClickEvent{static_cast<MouseButton>(button),
                            static_cast<KeyAction>(action), value});
    }
}

void InputEventManager::process_events() {
    process_keyboard_events();
    process_mouse_events();
    JoystickInputManager::update_key_data();
    AxisManager::update_axis_values();
}

void InputEventManager::process_keyboard_events() {
    auto& keys = RawInput::get_all_keys();

    // Reset 'changed' value for each key state
    for (auto& [key, state] : keys) { state.has_changed = false; }

    // While there are events in the event queue
    while (!events.key_events.empty()) {
        // Get the first event in the queue
        KeyEvent& event = events.key_events.front();
        KeyState& state = keys[event.key];

        ActionBindingManager::trigger_if_event(event);

        // Since there was an event involving this key, it's state has changed:
        state.has_changed = true;
        if (event.action == KeyAction::Press) {
            state.down = true;
        } else if (event.action == KeyAction::Release) {
            state.down = false;
        }

        // For now, raw_value and value are essentially the same.
        state.value = state.raw_value = event.value;

        // Remove the event from the queue
        events.key_events.pop();
    }
}

void InputEventManager::process_mouse_events() {
    // First, reset the mouse state
    auto& mouse = RawInput::get_mouse();
    mouse.xoffset = 0.0f;
    mouse.yoffset = 0.0f;
    mouse.wheel = 0.0f;
    mouse.has_changed = false;

    // Process all mouse events
    while (!events.mouse_events.empty()) {
        MouseEvent& event = events.mouse_events.front();

        // Set the mouse's state
        mouse.has_changed = true;
        mouse.xoffset = event.mouse_x;
        mouse.yoffset = event.mouse_y;
        mouse.wheel = event.wheel;

        // Remove the event from the queue
        events.mouse_events.pop();
    }

    // Process mouse click events
    auto& mouse_buttons = RawInput::get_all_mouse_buttons();
    // Reset all 'has_changed' values
    for (auto& [button, state] : mouse_buttons) { state.has_changed = false; }
    // Process the events
    while (!events.mouse_click_events.empty()) {
        MouseClickEvent& event = events.mouse_click_events.front();
        KeyState& state = mouse_buttons[event.button];

        // Set the state
        state.has_changed = true;
        if (event.action == KeyAction::Press) {
            state.down = true;
        } else {
            state.down = false;
        }
        state.value = event.value;

        events.mouse_click_events.pop();
    }
}

std::unordered_map<JoystickId, JoystickInputManager::JoystickData>
    JoystickInputManager::present_joysticks;

void JoystickInputManager::find_present_joysticks() {
    for (int raw_id = GLFW_JOYSTICK_1; raw_id <= GLFW_JOYSTICK_LAST; ++raw_id) {
        auto id = static_cast<JoystickId>(raw_id);
        int present = glfwJoystickPresent(raw_id);
        if (present == 1) {
            auto& joystick = present_joysticks[id];
            joystick.connected = true;
            joystick.is_gamepad = glfwJoystickIsGamepad(raw_id);
            joystick.name = glfwGetJoystickName(raw_id);
            if (joystick.is_gamepad) {
                joystick.gamepad_name = glfwGetGamepadName(raw_id);
            }
            LogSystem::write(LogSystem::Severity::Info,
                             "Joystick '" + joystick.name +
                                 "' detected in slot " +
                                 std::to_string(raw_id - GLFW_JOYSTICK_1));
        } else if (present == 0) {
            present_joysticks[id].connected = false;
        }
    }
}

void JoystickInputManager::joystick_connection_callback(int raw_id, int event) {
    auto id = static_cast<JoystickId>(raw_id);
    if (event == GLFW_CONNECTED) {
        auto& joystick = present_joysticks[id];
        joystick.connected = true;
        joystick.is_gamepad = glfwJoystickIsGamepad(raw_id);
        joystick.name = glfwGetJoystickName(raw_id);
        if (joystick.is_gamepad) {
            joystick.gamepad_name = glfwGetGamepadName(raw_id);
        }
        LogSystem::write(LogSystem::Severity::Info,
                         "Joystick '" + joystick.name + "' connected to slot " +
                             std::to_string(raw_id - GLFW_JOYSTICK_1));
    } else if (event == GLFW_DISCONNECTED) {
        present_joysticks[id].connected = false;
        LogSystem::write(LogSystem::Severity::Info,
                         "Joystick '" + present_joysticks[id].name +
                             "' disconnected from slot " +
                             std::to_string(raw_id - GLFW_JOYSTICK_1));
    }
}

void JoystickInputManager::update_key_data() {
    auto& keys = RawInput::get_all_keys();
    // #SingleJoystick (Update later)
    if (!present_joysticks[JoystickId::Joystick1].connected) { return; }
    // #TODO currently we only support input from joysticks that are
    // gamepads
    if (!present_joysticks[JoystickId::Joystick1].is_gamepad) { return; }

    auto& joystick = present_joysticks[JoystickId::Joystick1];

    GLFWgamepadstate glfw_state;
    if (!glfwGetGamepadState(static_cast<int>(JoystickId::Joystick1),
                             &glfw_state)) {
        return;
    }

    for (int btn = GLFW_GAMEPAD_BUTTON_A; btn <= GLFW_GAMEPAD_BUTTON_LAST;
         ++btn) {
        auto button = static_cast<Key>(btn);
        keys[button].down = glfw_state.buttons[btn];
        keys[button].raw_value = keys[button].value = glfw_state.buttons[btn];
    }

    for (int ax = GLFW_GAMEPAD_AXIS_LEFT_X; ax <= GLFW_GAMEPAD_AXIS_LAST;
         ++ax) {
        auto axis = static_cast<Key>(ax);
        auto val = glfw_state.axes[ax];
		static constexpr float epsilon = 0.1f;
        if (val > epsilon || val < -epsilon) {
            keys[axis].down = true;
        } else {
            keys[axis].down = false;
            val = 0.0f;
        }
        keys[axis].value = keys[axis].raw_value = val;
    }
}

std::vector<ActionBinding> ActionBindingManager::actions;

void ActionBindingManager::add_action(ActionBinding const& action) {
    actions.push_back(action);
}

void ActionBindingManager::trigger_if_event(
    InputEventManager::KeyEvent const& event) {
    for (auto const& action : actions) {
        // Check if the action triggers
        if (event.key == action.key && event.action == action.when) {
            // Call the callback function
            action.callback();
        }
    }
}

float Input::get_axis(std::string const& name) {
    auto const& axes = AxisManager::get_axes();
    std::size_t id = AxisManager::get_axis_id(name);
    for (auto const& axis : axes) {
        if (axis.id == id) { return axis.value; }
    }

    LogSystem::write(LogSystem::Severity::Warning,
                     "No axis exists with name " + name);
    return 0.0f;
}

float Input::get_axis_raw(std::string const& name) {
    auto const& axes = AxisManager::get_axes();
    std::size_t id = AxisManager::get_axis_id(name);
    for (auto const& axis : axes) {
        if (axis.id == id) { return axis.raw_value; }
    }
    LogSystem::write(LogSystem::Severity::Warning,
                     "No axis exists with name " + name);
    return 0.0f;
}

} // namespace Saturn
