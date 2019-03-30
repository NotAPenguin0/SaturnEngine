#include "Engine.hpp"

#include <thread>

namespace Saturn {

Application Engine::initialize(CreateInfo create_info) {
    // Initialize logging system
    LogSystem::initialize(create_info.log_target);
    // Initialize GLFW
    if (!glfwInit()) {
        LogSystem::write(LogSystem::Severity::FatalError,
                         "Failed to initialize GLFW.");
        // This is a fatal error, so exit.
        safe_terminate();
    }

    // Initialize error handling subsystem
    GLErrorHandler::initialize();

    // Set window hints
    glfwWindowHint(GLFW_SAMPLES, create_info.samples);
    // The engine uses OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    /*Mac OS X needs this line of code to initialize*/
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // The next step is to create the Application class.
    Application app(create_info.app_create_info);

    // The Application is initialized. This means we have an active context and
    // we can load GLAD

    // These pragmas are to ignore the -Wold-style-cast warning clang gives on
    // this line.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
#pragma clang diagnostic pop
        app.quit();
        glfwTerminate();
        LogSystem::write(LogSystem::Severity::FatalError,
                         "Failed to initialize GLAD.");
        safe_terminate();
    }
    // Register window callbacks
    glfwSetFramebufferSizeCallback(app.window_handle,
                                   Application::resize_callback);

    // Enable some OpenGL functionality we're going to need
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize subsystems. This process is multithreaded for all subsystems.
    std::thread input_init([&app]() {
        Input::initialize(app);
        // Bind the escape key to quit. This may change in the future or be
        // configurable somewhere
        Input::bind(GLFW_KEY_ESCAPE, [&app]() { app.quit(); });
    });

    std::thread renderer_init([&app, &create_info]() {
        Renderer::CreateInfo renderer_create_info{
            create_info.app_create_info.window_size, app};
        app.renderer = std::make_unique<Renderer>(renderer_create_info);
    });

	std::thread random_init(Math::RandomEngine::initialize);

    // Join all subsystem threads
    input_init.join();
    renderer_init.join();
    random_init.join();

    return app;
}

} // namespace Saturn
