#ifndef SATURN_INPUT_HPP_
#define SATURN_INPUT_HPP_

#include <functional>
#include <unordered_map>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Core/Application.hpp"

namespace Saturn {

class Application;

class Input {
public:
    enum class Action { Scroll, MouseMove, Keypress };

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

} // namespace Saturn

#endif
