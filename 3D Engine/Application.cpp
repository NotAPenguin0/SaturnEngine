#include "Application.hpp"

#include "LogSystem.hpp"

namespace Saturn {

Application::Application(CreateInfo create_info) :
    window_handle(nullptr), window_dimensions(create_info.window_size) {

    // If fullscreen is true, set this to glfwGetPrimaryMonitor() to enable
    // fullscreen. If it's nullptr, fullscreen will be disabled
    GLFWmonitor* monitor = nullptr;
    if (create_info.fullscreen) monitor = glfwGetPrimaryMonitor();

    // Create the window.The last parameter is nullptr because it's
    // not used in this Engine
    window_handle =
        glfwCreateWindow(create_info.window_size.x, create_info.window_size.y,
                         create_info.window_caption.data(), monitor, nullptr);
    if (window_handle == nullptr) {
        // Fatal error
        LogSystem::write(LogSystem::Severity::FatalError,
                         "Failed to create window.");
        exit(-1);
    }

    glfwMakeContextCurrent(window_handle);
}

Application::Application(Application&& other) :
    window_dimensions(other.window_dimensions),
    window_handle(other.window_handle), window_is_open(other.window_is_open) {

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
    if (window_handle != nullptr) {
        glfwSetWindowShouldClose(window_handle, true);
    }
}

} // namespace Saturn
