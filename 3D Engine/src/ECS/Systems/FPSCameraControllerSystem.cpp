#include "ECS/Systems/FPSCameraControllerSystem.hpp"

#include "ECS/Components/Camera.hpp"
#include "ECS/Components/FPSCameraController.hpp"
#include "ECS/Components/Transform.hpp"

#include "Input/Input.hpp"
#include "Scene/Scene.hpp"
#include "Time/Time.hpp"

#include "Renderer/OpenGL.hpp"

#include "Editor/EditorLog.hpp"

namespace Saturn {

namespace Systems {

void FPSCameraControllerSystem::on_start(Scene&) {}

void FPSCameraControllerSystem::on_update(Scene& scene) {
    auto& ecs = scene.get_ecs();

    for (auto [trans, cam, controller] :
         ecs.select<Components::Transform, Components::Camera,
                    Components::FPSCameraController>()) {

        float speed = controller.speed * Time::deltaTime;

        float horizontal = Input::get_axis("Horizontal");
        float vertical = Input::get_axis("Vertical");

        glm::vec3 cam_right = glm::normalize(glm::cross(cam.front, cam.up));
        trans.position += (speed * cam.front * vertical);
        trans.position += (speed * cam_right * horizontal);
        trans.position.y += speed * Input::get_axis_raw("Up");
    }
}

} // namespace Systems

} // namespace Saturn
