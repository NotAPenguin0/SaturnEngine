#include "ECS/Systems/FPSSoundListenerSystem.hpp"

#include "Scene/Scene.hpp"

#include "ECS/Components/Camera.hpp"
#include "ECS/Components/SoundListener.hpp"

namespace Saturn::Systems {

void FPSSoundListenerSystem::on_update(Scene& scene) {
    using namespace Components;
    auto& ecs = scene.get_ecs();

	for (auto[cam, listener] : ecs.select<Camera, SoundListener>()) {
		listener.forward = cam.front;
		listener.position = glm::vec3(0, 0, 0);
	}
}

} // namespace Saturn::Systems
