#include "Input.hpp"

#include "Application.hpp"

namespace Saturn {

std::unordered_map<Input::KeyT, Input::CallbackT> Input::keybinds;
Application* Input::app = nullptr;

void Input::initialize(Application& program) { app = &program; }

void Input::update() {
    for (auto& [k, cb] : keybinds) {
        if (glfwGetKey(app->window(), k) == GLFW_PRESS) {
            // Call the callback function the user specified
            cb();
        }
    }
}

bool Input::key_pressed(KeyT key) {
    return glfwGetKey(app->window(), key) == GLFW_PRESS;
}

} // namespace Saturn
