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
        if (Input::key_pressed(GLFW_KEY_W)) {
            trans.position += (speed * cam.front);
        }
        if (Input::key_pressed(GLFW_KEY_S)) {
            trans.position -= (speed * cam.front);
        }
        if (Input::key_pressed(GLFW_KEY_A)) {
            trans.position -=
                (speed * glm::normalize(glm::cross(cam.front, cam.up)));
        }
        if (Input::key_pressed(GLFW_KEY_D)) {
            trans.position +=
                (speed * glm::normalize(glm::cross(cam.front, cam.up)));
        }
        if (Input::key_pressed(GLFW_KEY_SPACE)) { trans.position.y += speed; }
        if (Input::key_pressed(GLFW_KEY_LEFT_SHIFT)) {
            trans.position.y -= speed;
        }
    }
}

} // namespace Systems

} // namespace Saturn
