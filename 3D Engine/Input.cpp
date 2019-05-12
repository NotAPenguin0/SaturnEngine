#include "Input.hpp"

#include "Application.hpp"

namespace Saturn {

std::unordered_map<Input::KeyT, Input::CallbackT> Input::keybinds;
Application* Input::app = nullptr;

Input::MouseData Input::current = {0.0f, 0.0f, 0.0f, 0.0f};
Input::MouseData Input::previous = {0.0f, 0.0f, 0.0f, 0.0f};

void Input::initialize(Application& program) {
    app = &program;
    // center mouse
    current = previous =
        MouseData{program.size().x / 2.0f, program.size().y / 2.0f, 0.0f, 0.0f};

    glfwSetScrollCallback(program.window(), &Input::scroll_callback);
}

void Input::update() {
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
    glfwSetCursorPosCallback(app->window(), &Input::mouse_callback);
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

} // namespace Saturn
