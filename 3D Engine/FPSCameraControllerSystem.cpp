#include "FPSCameraControllerSystem.hpp"

#include "ECS.hpp"
#include "Input.hpp"
#include "VecMath.hpp"

#include "Math.hpp"
#include "Scene.hpp"

#include "Components.hpp"

namespace Saturn::Systems {

void FPSCameraControllerSystem::on_update(Scene& scene) {
    using namespace Components;

    for (auto [transform, camera, controller] :
         scene.get_ecs().select<Transform, Camera, FPSCameraController>()) {

        // With temporary input system. Needs replacing later
        //#TempInputSystem
        if (Input::key_pressed(GLFW_KEY_W)) {
            transform.position += controller.speed * camera.front;
        }
        if (Input::key_pressed(GLFW_KEY_S)) {
            transform.position -= controller.speed * camera.front;
        }
        if (Input::key_pressed(GLFW_KEY_A)) {
            transform.position -=
                controller.speed *
                Math::normalize(Math::cross(camera.front, camera.up));
        }
        if (Input::key_pressed(GLFW_KEY_D)) {
            transform.position +=
                controller.speed *
                Math::normalize(Math::cross(camera.front, camera.up));
        }
    }
}

} // namespace Saturn::Systems
