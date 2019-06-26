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
#include "Utility/Utility.hpp"

#include <thread>
#include <chrono>

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
	window_is_open = true;
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

static SceneObject& create_particle_emitter(std::string_view path,
                                            Scene& scene) {
    auto& obj = scene.create_object_from_file(path);
    auto& emitter = obj.get_component<Components::ParticleEmitter>();
    VertexArray::CreateInfo vao_info;
    vao_info.attributes.push_back({0, 3}); // position
    vao_info.attributes.push_back({1, 2}); // texture coordinates
    vao_info.vertices = particle_quad_vertices;
    vao_info.indices = particle_quad_indices;
    emitter.particle_vao =
        AssetManager<VertexArray>::get_resource(vao_info, "particle_vao");
    VertexArray::BufferInfo pos_buffer_info;
    pos_buffer_info.attributes.push_back({2, 3, 1}); // position
    pos_buffer_info.mode = BufferMode::DataStream;
    //#TODO: Get rid of make_float_vec
    pos_buffer_info.data = make_float_vec(emitter.particle_data.positions);
    VertexArray::BufferInfo scale_buffer_info;
    scale_buffer_info.attributes.push_back({3, 3, 1}); // scale
    scale_buffer_info.mode = BufferMode::DataStream;
    scale_buffer_info.data = make_float_vec(emitter.particle_data.sizes);
    VertexArray::BufferInfo color_buffer_info;
    color_buffer_info.attributes.push_back({4, 4, 1}); // color
    color_buffer_info.mode = BufferMode::DataStream;
    color_buffer_info.data = make_float_vec(emitter.particle_data.colors);

    emitter.particle_vao->add_buffer(pos_buffer_info);
    emitter.particle_vao->add_buffer(scale_buffer_info);
    emitter.particle_vao->add_buffer(color_buffer_info);
    return obj;
}

void Application::run() {
    Scene scene;

    // Add systems
    scene.ecs.register_system<Systems::FPSCameraControllerSystem>();
    scene.ecs.register_system<Systems::CameraZoomControllerSystem>();
    scene.ecs.register_system<Systems::FreeLookControllerSystem>();
    scene.ecs.register_system<Systems::ParticleSystem>();
    scene.ecs.register_system<Systems::RotatorSystem>();

    auto& x = scene.create_object();
    {
        auto id = x.add_component<Components::Transform>();
        auto& transform = scene.ecs.get_with_id<Components::Transform>(id);
        transform.position.y = -2.0f;
    }

    auto& obj =
        create_particle_emitter("resources/entities/my_emitter.json", scene);

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

    Input::bind(GLFW_KEY_F, [&scene]() {
        scene.serialize_to_file("resources/scene0");
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(100ms);
    });

    scene.on_start();
    while (!glfwWindowShouldClose(window_handle)) {
        Time::update();
        Input::update();

        renderer->clear(Color{0.24f, 0.0f, 0.0f, 1.0f});

        scene.update_systems();
        renderer->render_scene(scene);

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
