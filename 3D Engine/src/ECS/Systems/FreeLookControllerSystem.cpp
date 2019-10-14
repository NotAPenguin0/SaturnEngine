#include "ECS/Systems/FreeLookControllerSystem.hpp"

// Engine subsystems

#include "Input/Input.hpp"
#include "Scene/Scene.hpp"
#include "Time/Time.hpp"

// Components

#include "ECS/Components/Camera.hpp"
#include "ECS/Components/FreeLookController.hpp"
#include "ECS/Components/Transform.hpp"

namespace Saturn::Systems {

void FreeLookControllerSystem::on_update(Scene& scene) {
    using namespace Components;

    auto& ecs = scene.get_ecs();

    for (auto [transform, cam, controller] :
         ecs.select<Transform, Camera, FreeLookController>()) {

        float xoffset = RawInput::get_mouse().xoffset;
        float yoffset = RawInput::get_mouse().yoffset;
        xoffset *= controller.mouse_sensitivity; //* Time::deltaTime;
        yoffset *= controller.mouse_sensitivity; //* Time::deltaTime;

        // x and y flipped. This is not a bug, since rotation around y means
        // looking around left/right, which is controlled by moving the mouse
        // left and right
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
}

} // namespace Saturn::Systems