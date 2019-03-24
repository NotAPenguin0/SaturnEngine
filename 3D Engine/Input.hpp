#ifndef SATURN_INPUT_HPP_
#define SATURN_INPUT_HPP_

#include <functional>
#include <unordered_map>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Application.hpp"

namespace Saturn {

class Application;

class Input {
public:
    enum class Action { Scroll, MouseMove, Keypress };

    using CallbackT = std::function<void()>;
    using KeyT = int;

    // \brief Initializes the Input system. Stores a pointer to the program
    // passed in
    static void initialize(Application& program);

    template<typename Callback>
    static void bind(KeyT key, Callback&& func) {
        keybinds[key] = std::forward<Callback>(func);
    }

    static bool key_pressed(KeyT key);
    static void update();

private:
    static std::unordered_map<KeyT, CallbackT> keybinds;
    static Application* app;
};

} // namespace Saturn

#endif
