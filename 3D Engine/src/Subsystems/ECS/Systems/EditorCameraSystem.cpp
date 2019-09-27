#include "Subsystems/ECS/Systems/EditorCameraSystem.hpp"

#include "Subsystems/ECS/Components/Camera.hpp"
#include "Subsystems/ECS/Components/EditorCameraController.hpp"
#include "Subsystems/ECS/Components/Transform.hpp"

#include "Editor/EditorLog.hpp"

#include "Subsystems/Input/Input.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Time/Time.hpp"

namespace Saturn::Systems {

void EditorCameraSystem::on_update(Scene& scene) {
    using namespace Components;
    for (auto [transform, controller, cam] :
         scene.get_ecs().select<Transform, EditorCameraController, Camera>()) {
        // Only activate these controls if the right mouse button is down
        if (RawInput::get_mouse_button(MouseButton::Right).down) {
            freelook(transform, cam, controller);
        }
        if (RawInput::get_mouse_button(MouseButton::Left).down &&
            RawInput::get_key(Key::Space).down) {
            movement(transform, cam, controller);
        }
		zoom(cam, controller);
    }
}

void EditorCameraSystem::freelook(
    Components::Transform& transform,
    Components::Camera& cam,
    Components::EditorCameraController& controller) {

    float xoffset = RawInput::get_mouse().xoffset;
    float yoffset = RawInput::get_mouse().yoffset;
    xoffset *= controller.sensitivity;
    yoffset *= controller.sensitivity;

    transform.rotation.x += yoffset;
    transform.rotation.y += xoffset;
    static float max_pitch = 89.0f;
    static float min_pitch = -89.0f;
    if (transform.rotation.x > max_pitch) transform.rotation.x = max_pitch;
    if (transform.rotation.x < min_pitch) transform.rotation.x = min_pitch;

    // Rotation
    auto cos_pitch = std::cos(glm::radians(transform.rotation.x));
    auto cos_yaw = std::cos(glm::radians(transform.rotation.y));
    auto sin_pitch = std::sin(glm::radians(transform.rotation.x));
    auto sin_yaw = std::sin(glm::radians(transform.rotation.y));
    cam.front.x = cos_pitch * cos_yaw;
    cam.front.y = sin_pitch;
    cam.front.z = cos_pitch * sin_yaw;
    cam.front = glm::normalize(cam.front);
}

void EditorCameraSystem::movement(
    Components::Transform& trans,
    Components::Camera& cam,
    Components::EditorCameraController& controller) {

    float speed = controller.speed * Time::deltaTime;

    float horizontal = RawInput::get_mouse().xoffset * pow(-1, controller.invert_x);
    float vertical = RawInput::get_mouse().yoffset * pow(-1, controller.invert_y);

    glm::vec3 cam_right = glm::normalize(glm::cross(cam.front, cam.up));
    trans.position -= (speed * cam.front * vertical);
    trans.position -= (speed * cam_right * horizontal);
}

void EditorCameraSystem::zoom(Components::Camera& cam,
                             Components::EditorCameraController& controller) {
    auto mouse = RawInput::get_mouse();

    auto min = 1.0f;
    auto max = 45.0f;

    if (cam.fov >= min && cam.fov <= max) {
        cam.fov -= mouse.wheel * controller.zoom_speed * Time::deltaTime;
    }
    if (cam.fov <= min) { cam.fov = min;}
    if (cam.fov >= max) { cam.fov = max; }
}

} // namespace Saturn::Systems
