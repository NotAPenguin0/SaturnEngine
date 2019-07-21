#include "Subsystems/Input/Input.hpp"

#include "Core/Application.hpp"

namespace Saturn {

std::unordered_map<Input::KeyT, Input::CallbackT> Input::keybinds;
Application* Input::app = nullptr;

Input::MouseData Input::current = {0.0f, 0.0f, 0.0f, 0.0f};
Input::MouseData Input::previous = {0.0f, 0.0f, 0.0f, 0.0f};

void Input::initialize(Application& program) {
    InputEventManager::init(program);

    app = &program;
    // center mouse
    current = previous =
        MouseData{program.size().x / 2.0f, program.size().y / 2.0f, 0.0f, 0.0f};
}

void Input::update() {
    InputEventManager::process_events();
    for (auto& [k, cb] : keybinds) {
        if (glfwGetKey(app->window(), k) == GLFW_PRESS) {
            // Call the callback function the user specified
            cb();
        }
    }
}

void Input::tick_end() {
    previous = current;
    current.xscroll = 0.0f;
    current.yscroll = 0.0f;
}

bool Input::key_pressed(KeyT key) {
    return glfwGetKey(app->window(), key) == GLFW_PRESS;
}

void Input::enable_mouse_capture() {
    glfwSetInputMode(app->window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Input::MouseData Input::mouse() { return current; }
Input::MouseData Input::previous_mouse() { return previous; }

void Input::mouse_callback([[maybe_unused]] GLFWwindow* window,
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

void Input::scroll_callback([[maybe_unused]] GLFWwindow* window,
                            double xoffset,
                            double yoffset) {

    previous.xscroll = current.xscroll;
    previous.yscroll = current.yscroll;

    current.xscroll = static_cast<float>(xoffset);
    current.yscroll = static_cast<float>(yoffset);
}

std::unordered_map<Key, KeyState> RawInput::keys;
MouseState RawInput::mouse;

KeyState& RawInput::get_key(Key key) { return keys[key]; }

std::unordered_map<Key, KeyState>& RawInput::get_all_keys() { return keys; }

MouseState& RawInput::get_mouse() { return mouse; }

// Define static variables
InputEventManager::Events InputEventManager::events;
Application* InputEventManager::app;

void InputEventManager::init(Application& program) {
    glfwSetKeyCallback(program.window(), &InputEventManager::keyboard_callback);
    glfwSetCursorPosCallback(program.window(),
                             &InputEventManager::mouse_position_callback);
    glfwSetScrollCallback(program.window(),
                          &InputEventManager::scroll_callback);
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

void InputEventManager::scroll_callback(GLFWwindow* win, double xoffset, double yoffset) {
	events.mouse_events.push(MouseEvent{0.0f, 0.0f, static_cast<float>(yoffset)});
}

void InputEventManager::process_events() {
    process_keyboard_events();
    process_mouse_events();
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
}

} // namespace Saturn
