#include "Engine.hpp"

namespace Saturn {

Application Engine::initialize(CreateInfo create_info) {
    // Initialize logging system
    LogSystem::initialize(create_info.log_target);
    // Initialize GLFW
    if (!glfwInit()) {
        LogSystem::write(LogSystem::Severity::FatalError,
                         "Failed to initialize GLFW.");
        // This is a fatal error, so exit.
        exit(-1);
    }

    // The next step is to create the Application class.
    Application app(create_info.app_create_info);
}

} // namespace Saturn
