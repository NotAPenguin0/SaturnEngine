#include "CameraZoomControllerSystem.hpp"

#include "Input.hpp"
#include "Scene.hpp"
#include "Time.hpp"

#include "Camera.hpp"
#include "CameraZoomController.hpp"

namespace Saturn::Systems {

void CameraZoomControllerSystem::on_update(Scene& scene) {
    for (auto [cam, zoom_controller] :
         scene.get_ecs()
             .select<Components::Camera, Components::CameraZoomController>()) {

        auto mouse = Input::mouse();

        auto& min = zoom_controller.min_zoom;
        auto& max = zoom_controller.max_zoom;

        if (cam.fov >= min && cam.fov <= max) {
            cam.fov -=
                mouse.yscroll * zoom_controller.zoom_speed * Time::deltaTime;
        }
        if (cam.fov <= min) { cam.fov = max; }
        if (cam.fov >= max) { cam.fov = max; }
    }
}

} // namespace Saturn::Systems
