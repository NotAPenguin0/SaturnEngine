#include "Core/Application.hpp"

#include "Subsystems/ECS/Components.hpp"
#include "Subsystems/ECS/Systems.hpp"
#include "Subsystems/Input/Input.hpp"
#include "Subsystems/Logging/LogSystem.hpp"
#include "Subsystems/Math/Math.hpp"
#include "Subsystems/Renderer/Viewport.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Scene/SceneObject.hpp"
#include "Subsystems/Time/Time.hpp"

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
    Scene scene;

    // Add systems
    scene.ecs.register_system<Systems::FPSCameraControllerSystem>();
    scene.ecs.register_system<Systems::CameraZoomControllerSystem>();
    scene.ecs.register_system<Systems::FreeLookControllerSystem>();

    auto& obj = scene.create_object(); 
    auto transform_id = obj.add_component<Components::Transform>();
    {
        auto& transform =
            scene.ecs.get_with_id<Components::Transform>(transform_id);
        transform.position = glm::vec3(5.0f, 0.0f, 0.0f);
        transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        auto& mesh = scene.ecs.get_with_id<Components::StaticMesh>(
            obj.add_component<Components::StaticMesh>());
        mesh.mesh =
            AssetManager<Mesh>::get_resource("resources/meshes/my_cube.mesh");
    }

    auto& main_cam = scene.create_object();
    {
        auto& transform = scene.ecs.get_with_id<Components::Transform>(
            main_cam.add_component<Components::Transform>());
        auto& camera = scene.ecs.get_with_id<Components::Camera>(
            main_cam.add_component<Components::Camera>());
        auto& fps = scene.ecs.get_with_id<Components::FPSCameraController>(
            main_cam.add_component<Components::FPSCameraController>());
        auto& freelook = scene.ecs.get_with_id<Components::FreeLookController>(
            main_cam.add_component<Components::FreeLookController>());
        auto& zoom = scene.ecs.get_with_id<Components::CameraZoomController>(
            main_cam.add_component<Components::CameraZoomController>());

        transform.position = {0.0f, 0.0f, 0.0f};

        camera.front = {1.0f, 0.0f, 0.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fov = 45.0f;

        fps.speed = 2.5f;
        freelook.mouse_sensitivity = 0.08f;
        zoom.zoom_speed = 100.0f;

        renderer->get_viewport(0).set_camera(camera.id);
    }

    scene.on_start();
    while (!glfwWindowShouldClose(window_handle)) {
        Time::update();
        Input::update();

        renderer->clear(Color{0.24f, 0.0f, 0.0f, 1.0f});

        scene.update_systems();
        auto graph = scene.build_scene_graph();
        renderer->render_scene_graph(graph);

        // Copy framebuffer to screen
        renderer->update_screen();

        Input::tick_end();

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
    Viewport::set_active(Viewport(0u, 0u, static_cast<unsigned int>(w),
                                  static_cast<unsigned int>(h)));
}

GLFWwindow* Application::window() { return window_handle; }

WindowDim Application::size() const { return window_dimensions; }

} // namespace Saturn
