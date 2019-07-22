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
    InputOld::enable_mouse_capture();
}

void FPSCameraControllerSystem::on_update(Scene& scene) {
    auto& ecs = scene.get_ecs();

    for (auto [trans, cam, controller] :
         ecs.select<Components::Transform, Components::Camera,
                    Components::FPSCameraController>()) {

        float speed = controller.speed * Time::deltaTime;

		/*
        if (RawInput::get_mouse_button(MouseButton::Left).down) {
            speed += 5.0f * Time::deltaTime;
        }*/

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
