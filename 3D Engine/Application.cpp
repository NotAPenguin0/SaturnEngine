#include "Application.hpp"

#include "Input.hpp"
#include "LogSystem.hpp"
#include "Scene.hpp"
#include "SceneObject.hpp"
#include "Systems.hpp"
#include "Trig.hpp"
#include "VecMath.hpp"
#include "Vector.hpp"
#include "Viewport.hpp"

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

    auto& obj = scene.create_object();
    auto transform_id = obj.add_component<Components::Transform>();
    {
        auto& transform =
            scene.ecs.get_with_id<Components::Transform>(transform_id);
        transform.position = Math::Vec3<float>(0.0f, 0.0f, -10.0f);
        transform.rotation.axis = Math::Vec3<float>(1.0f, 0.0f, 0.0f);
        transform.rotation.angle_in_radians = Math::radians(0.0f);
        transform.scale = Math::Vec3<float>(1.0f, 1.0f, 1.0f);
    }

    // Double viewport
    /*
    auto& left_camera = scene.create_object();
    auto lcam_transform_id = left_camera.add_component<Components::Transform>();
    auto lcam_id = left_camera.add_component<Components::Camera>();
    {
        auto& lcam_transform =
            scene.ecs.get_with_id<Components::Transform>(lcam_transform_id);
        auto& lcam = scene.ecs.get_with_id<Components::Camera>(lcam_id);
        lcam_transform.position.x = 0.0f;
        lcam_transform.position.y = 0.0f;
        lcam_transform.position.z = 0.0f;
        lcam.target = {0.0f, 0.0f, -10.0f};

        lcam_transform.scale = {
            0.0f, 0.0f, 0.0f}; // Temporary disable the model in
                               // SceneObject, by scaling it with a factor 0
    }

    auto& right_camera = scene.create_object();
    auto rcam_transform_id =
    right_camera.add_component<Components::Transform>(); auto rcam_id =
    right_camera.add_component<Components::Camera>();
    {
        auto& rcam_transform =
            scene.ecs.get_with_id<Components::Transform>(rcam_transform_id);
        auto& rcam = scene.ecs.get_with_id<Components::Camera>(rcam_id);
        rcam_transform.position.x = 0.0f;
        rcam_transform.position.y = 0.0f;
        rcam_transform.position.z = 0.0f;
        rcam.target = {0.0f, 0.0f, -10.0f};

        rcam_transform.scale = {0.0f, 0.0f, 0.0f};
    }

    auto left_vp = renderer->add_viewport(Viewport(0, 0, window_dimensions.x /
    2, window_dimensions.y));
    renderer->get_viewport(left_vp).set_camera(lcam_id);

    auto right_vp = renderer->add_viewport(Viewport(window_dimensions.x / 2, 0,
                                                    window_dimensions.x / 2,
                                                    window_dimensions.y));
    renderer->get_viewport(right_vp).move(0, 0);
    renderer->get_viewport(right_vp).resize(window_dimensions.x,
                                            window_dimensions.y);
    renderer->get_viewport(right_vp).set_camera(rcam_id);
    */

    auto& main_cam = scene.create_object();
    {
        auto& transform = scene.ecs.get_with_id<Components::Transform>(
            main_cam.add_component<Components::Transform>());
        auto& camera = scene.ecs.get_with_id<Components::Camera>(
            main_cam.add_component<Components::Camera>());
        auto& fps = scene.ecs.get_with_id<Components::FPSCameraController>(
            main_cam.add_component<Components::FPSCameraController>());
        transform.position = {0.0f, 3.0f, 0.0f};
        transform.scale = {0.0f, 0.0f, 0.0f};

        camera.front = {0.0f, 0.0f, -1.0f};
        camera.up = {0.0f, 1.0f, 0.0f};

		fps.speed = 0.03f;

        renderer->get_viewport(0).set_camera(camera.id);
    }

    while (!glfwWindowShouldClose(window_handle)) {
        Input::update();

        renderer->clear(Color{0.24f, 0.0f, 0.0f, 1.0f});

		scene.update_systems();
        auto graph = scene.build_scene_graph();
        renderer->render_scene_graph(graph);

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
    Viewport::set_active(Viewport(0u, 0u, static_cast<unsigned int>(w),
                                  static_cast<unsigned int>(h)));
}

GLFWwindow* Application::window() { return window_handle; }

} // namespace Saturn
