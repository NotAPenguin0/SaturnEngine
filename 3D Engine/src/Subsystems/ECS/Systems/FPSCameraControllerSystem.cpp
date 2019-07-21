#include "Subsystems/ECS/Systems/FPSCameraControllerSystem.hpp"

#include "Subsystems/ECS/Components/Camera.hpp"
#include "Subsystems/ECS/Components/FPSCameraController.hpp"
#include "Subsystems/ECS/Components/Transform.hpp"

#include "Subsystems/Input/Input.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Time/Time.hpp"

#include "Subsystems/Renderer/OpenGL.hpp"

namespace Saturn {

namespace Systems {

void FPSCameraControllerSystem::on_start(Scene&) {
    Input::enable_mouse_capture();
}

void FPSCameraControllerSystem::on_update(Scene& scene) {
    auto& ecs = scene.get_ecs();

    for (auto [trans, cam, controller] :
         ecs.select<Components::Transform, Components::Camera,
                    Components::FPSCameraController>()) {

        float speed = controller.speed * Time::deltaTime;
        if (RawInput::get_key(Key::W).down) {
            trans.position += (speed * cam.front);
        }
        if (RawInput::get_key(Key::S).down) {
            trans.position -= (speed * cam.front);
        }
        if (RawInput::get_key(Key::A).down) {
            trans.position -=
                (speed * glm::normalize(glm::cross(cam.front, cam.up)));
        }
        if (RawInput::get_key(Key::D).down) {
            trans.position +=
                (speed * glm::normalize(glm::cross(cam.front, cam.up)));
        }
        if (RawInput::get_key(Key::Space).down) {
            trans.position.y += speed;
        }
        if (RawInput::get_key(Key::LeftShift).down) {
            trans.position.y -= speed;
        }
    }
}

} // namespace Systems

} // namespace Saturn
