#include "Engine.hpp"

#include <thread>

namespace Saturn {

namespace {

struct framebuffer_resize_callback {
    static Application* app;

    static void callback(GLFWwindow* win, int w, int h) {
        app->resize_callback(win, w, h);
    }
};

Application* framebuffer_resize_callback::app = nullptr;

} // namespace

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
    // The engine uses OpenGL 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,
                   create_info.enable_debug_output ? GL_TRUE : GL_FALSE);

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
    // Using a workaround because capturing lambdas do not convert to function
    // pointers
    framebuffer_resize_callback::app = &app;
    glfwSetFramebufferSizeCallback(app.window_handle,
                                   framebuffer_resize_callback::callback);

    // Enable some OpenGL functionality we're going to need
    //    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable debug output if specified
    if (create_info.enable_debug_output) {
        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(&GLErrorHandler::gl_error_callback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                                  nullptr, GL_TRUE);
        } else {
            LogSystem::write(LogSystem::Severity::Error,
                             "Couldn't make GL Debug Output");
        }
    }

    // Initialize subsystems. This process is multithreaded for all subsystems.
    std::thread input_init([&app]() {
        Input::initialize(app);
        // Bind the escape key to quit. This may change in the future or be
        // configurable somewhere
        Input::bind(GLFW_KEY_ESCAPE, [&app]() { app.quit(); });
    });

    std::thread random_init(Math::RandomEngine::initialize);

    // Single threaded Renderer initialization, because calling OpenGL functions
    // from a different thread isn't a good idea
    Renderer::CreateInfo renderer_create_info{
        create_info.app_create_info.window_size, app};
    app.renderer = std::make_unique<Renderer>(renderer_create_info);

    app.initialize_keybinds();

    // Join all subsystem threads
    input_init.join();
    random_init.join();

    return app;
}

} // namespace Saturn
