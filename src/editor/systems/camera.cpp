#include <editor/systems/camera.hpp>

#include <saturn/core/input.hpp>

#include <saturn/components/camera.hpp>
#include <saturn/components/transform.hpp>
#include <editor/components/editor_camera.hpp>

#include <phobos/core/vulkan_context.hpp>
#include <glm/glm.hpp>

namespace editor {

using namespace saturn::components;

static void do_freelook(saturn::FrameContext& ctx, Transform& transform, Camera& camera, EditorCamera& controller) {
    float xoffset = saturn::RawInput::get_mouse().xoffset * controller.sensitivity;
    float yoffset = saturn::RawInput::get_mouse().yoffset * controller.sensitivity;

    // Note that x and y are flipped. This is intentional
    transform.rotation.x += yoffset * ctx.delta_time;
    transform.rotation.y += xoffset * ctx.delta_time;
    constexpr float max_pitch = 89.0f;
    constexpr float min_pitch = -89.0f;
    if (transform.rotation.x > max_pitch) transform.rotation.x = max_pitch;
    if (transform.rotation.x < min_pitch) transform.rotation.x = min_pitch;

    float const cos_pitch = std::cos(glm::radians(transform.rotation.x));
    float const cos_yaw = std::cos(glm::radians(transform.rotation.y));
    float const sin_pitch = std::sin(glm::radians(transform.rotation.x));
    float const sin_yaw = std::sin(glm::radians(transform.rotation.y));
    camera.front.x = cos_pitch * cos_yaw;
    camera.front.y = sin_pitch;
    camera.front.z = cos_pitch * sin_yaw;
    camera.front = glm::normalize(camera.front);
}

static void do_movement(saturn::FrameContext& ctx, Transform& transform, Camera& camera, EditorCamera& controller) {
    float const speed = controller.sensitivity * ctx.delta_time;

    float const horizontal = saturn::RawInput::get_mouse().xoffset;
    float const vertical = saturn::RawInput::get_mouse().yoffset;

    // Editor camera controls correspond to how Unity implements it

    glm::vec3 right = glm::normalize(glm::cross(camera.front, camera.up));

    // If we move the mouse vertically, the camera should move along the up vector.
    // Note that the sign is flipped for more intuitive movement.
    transform.position -= vertical * speed * glm::normalize(glm::cross(camera.front, right));

    // If we move the mouse horizontally, the camera should move along the right vector.
    // Note that the sign is flipped for more intuitive movement.
    transform.position -= horizontal * speed * right;
}

void CameraSystem::startup(ph::VulkanContext&, saturn::Scene& scene) {
    // Add the editor camera controller to the main scene camera
    scene.ecs.add_component<EditorCamera>(scene.main_camera);
}

void CameraSystem::update(saturn::FrameContext& ctx) {
    for (auto[transform, camera, controller] : ctx.scene.ecs.view<Transform, Camera, EditorCamera>()) {
        if (saturn::RawInput::get_mouse_button(saturn::MouseButton::Right).down) {
            do_freelook(ctx, transform, camera, controller);
        }

        if (saturn::RawInput::get_mouse_button(saturn::MouseButton::Middle).down) {
            do_movement(ctx, transform, camera, controller);
        }
    }
}

}