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
    static float max_pitch = 89.0f;
    static float min_pitch = -89.0f;
    if (transform.rotation.x > max_pitch) transform.rotation.x = max_pitch;
    if (transform.rotation.x < min_pitch) transform.rotation.x = min_pitch;

    auto cos_pitch = std::cos(glm::radians(transform.rotation.x));
    auto cos_yaw = std::cos(glm::radians(transform.rotation.y));
    auto sin_pitch = std::sin(glm::radians(transform.rotation.x));
    auto sin_yaw = std::sin(glm::radians(transform.rotation.y));
    camera.front.x = cos_pitch * cos_yaw;
    camera.front.y = sin_pitch;
    camera.front.z = cos_pitch * sin_yaw;
    camera.front = glm::normalize(camera.front);
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
    }
}

}