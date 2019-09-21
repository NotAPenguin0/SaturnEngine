#include "Core/Application.hpp"

#include "Subsystems/ECS/Components.hpp"
#include "Subsystems/ECS/Systems.hpp"
#include "Subsystems/Input/Input.hpp"
#include "Subsystems/Logging/LogSystem.hpp"
#include "Subsystems/Math/Math.hpp"
#include "Subsystems/Physics/Physics.hpp"
#include "Subsystems/Renderer/Viewport.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Scene/SceneObject.hpp"
#include "Subsystems/Time/Time.hpp"
#include "Utility/Utility.hpp"

#include "audeo/audeo.hpp"

#include <chrono>
#include <thread>

#include "Editor/EditorLog.hpp"

#ifdef WITH_EDITOR
#    include "Editor/Editor.hpp"
#endif

namespace Saturn {

#ifdef WITH_EDITOR
void Application::set_editor_instance(Editor::Editor* a_editor) {
    editor = a_editor;
}
#endif

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
    window_is_open = true;

    physics = std::make_unique<Physics>();
    physics_scheduler.set_physics_system(*physics);

    audeo::InitInfo info;
    info.effect_channels = 32;
    if (audeo::init(info)) {
        log::log("Initialized audeo");
    } else {
        log::log("Failed to initialize audeo", Editor::DebugConsole::Error);
    }
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

    audeo::quit();
}

void Application::initialize_keybinds() {}

static const std::vector<float> particle_quad_vertices = {
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // TL
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // TR
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // BL
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f  // BR
};

static const std::vector<GLuint> particle_quad_indices = {
    0, 1, 2, // First triangle
    1, 2, 3  // Second triangle
};

void Application::run() {
    Scene scene(this);

    // Add systems
    scene.ecs.register_system<Systems::FPSCameraControllerSystem>();
    scene.ecs.register_system<Systems::CameraZoomControllerSystem>();
    scene.ecs.register_system<Systems::FreeLookControllerSystem>();
    scene.ecs.register_system<Systems::ParticleSystem>();
    scene.ecs.register_system<Systems::RotatorSystem>();
    scene.ecs.register_system<Systems::FlashlightSystem>();
    scene.ecs.register_system<Systems::ClickEffectSystem>();
	scene.ecs.register_system<Systems::FPSSoundListenerSystem>();
    scene.ecs.register_system<Systems::SoundSystem>();
#ifdef WITH_EDITOR
        scene.ecs.register_system<Systems::EditorCameraSystem>();
#endif

#ifdef WITH_EDITOR
    editor->setup_viewports();
#endif

    scene.deserialize_from_file("resources/scene1/scene.dat");
    scene.on_start();
    while (!glfwWindowShouldClose(window_handle)) {
        glfwPollEvents();
        Time::update();
        InputEventManager::process_events();
        SystemUpdateMode system_update_mode = SystemUpdateMode::Play;
#ifdef WITH_EDITOR
        editor->render(scene);
        system_update_mode = editor->get_update_mode();
        glViewport(0, 0, window_dimensions.x, window_dimensions.y);
#endif
        renderer->clear(Color{0.003f, 0.003f, 0.003f, 1.0f});
        glViewport(0, 0, 800, 600);

        scene.update_systems(system_update_mode);
        // This updates the timer in the physics scheduler, and runs a physics
        // tick if needed
        physics_scheduler.update(system_update_mode, scene);
        renderer->render_scene(scene);

        // Copy framebuffer to screen
        renderer->update_screen();
#ifdef WITH_EDITOR
        editor->frame_end();
#endif
        glfwSwapBuffers(window_handle);
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
    // #TODO: This should work correctly with the actual viewport system
    Viewport::set_active(Viewport(0u, 0u, static_cast<unsigned int>(w),
                                  static_cast<unsigned int>(h)));
}

GLFWwindow* Application::window() { return window_handle; }

WindowDim Application::size() const { return window_dimensions; }

} // namespace Saturn
