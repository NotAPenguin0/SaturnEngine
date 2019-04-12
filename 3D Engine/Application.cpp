#include "Application.hpp"

#include "Input.hpp"
#include "LogSystem.hpp"

namespace Saturn {

Application::Application(CreateInfo create_info) :
    window_handle(nullptr), window_dimensions(create_info.window_size) {

    // If fullscreen is true, set this to glfwGetPrimaryMonitor() to enable
    // fullscreen. If it's nullptr, fullscreen will be disabled
    GLFWmonitor* monitor = nullptr;
    if (create_info.fullscreen) { monitor = glfwGetPrimaryMonitor(); }

    // Create the window.The last parameter is nullptr because it's
    // not used in this Engine
    window_handle =
        glfwCreateWindow(create_info.window_size.x, create_info.window_size.y,
                         create_info.window_caption.data(), monitor, nullptr);
    if (window_handle == nullptr) {
        // Fatal error
        LogSystem::write(LogSystem::Severity::FatalError,
                         "Failed to create window.");
        safe_terminate();
    }

    glfwMakeContextCurrent(window_handle);
}

Application::Application(Application&& other) :
    window_handle(other.window_handle),
    window_dimensions(other.window_dimensions),
    window_is_open(other.window_is_open) {

    other.window_handle = nullptr;
    other.window_is_open = false;
}

Application& Application::operator=(Application&& other) {
    window_dimensions = other.window_dimensions;
    window_handle = other.window_handle;
    window_is_open = other.window_is_open;

    other.window_is_open = false;
    other.window_handle = nullptr;

    return *this;
}

Application::~Application() {
    if (window_handle != nullptr && !window_is_open) {
        glfwSetWindowShouldClose(window_handle, true);
    }
}

void Application::initialize_keybinds() {}

void Application::run() {
    while (!glfwWindowShouldClose(window_handle)) {
        Input::update();

        renderer->clear(Color{0.1f, 0.1f, 0.1f, 1.0f});

        // Copy framebuffer to screen
        renderer->update_screen();
        glfwSwapBuffers(window_handle);
        glfwPollEvents();
    }
}

void Application::quit() {
    if (window_handle != nullptr) {
        glfwSetWindowShouldClose(window_handle, true);
        window_is_open = false;
    }
}

void Application::resize_callback([[maybe_unused]] GLFWwindow* window,
                                  int w,
                                  int h) {
    // Set the viewport correctly
    Viewport::set_active(Viewport(0u, 0u, static_cast<unsigned int>(w), static_cast<unsigned int>(h)));
}

GLFWwindow* Application::window() { return window_handle; }

} // namespace Saturn
