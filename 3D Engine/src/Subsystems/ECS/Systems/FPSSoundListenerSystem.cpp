#include "Subsystems/ECS/Systems/FPSSoundListenerSystem.hpp"

#include "Subsystems/Scene/Scene.hpp"

#include "Subsystems/ECS/Components/Camera.hpp"
#include "Subsystems/ECS/Components/SoundListener.hpp"

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
