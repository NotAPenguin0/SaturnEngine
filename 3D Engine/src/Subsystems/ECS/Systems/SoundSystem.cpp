#include "Subsystems/ECS/Systems/SoundSystem.hpp"

#include "Subsystems/Scene/Scene.hpp"
#include "Utility/Utility.hpp"

#include "Subsystems/ECS/Components/Sound.hpp"
#include "Subsystems/ECS/Components/SoundListener.hpp"
#include "Subsystems/ECS/Components/Transform.hpp"

namespace Saturn::Systems {

void SoundSystem::on_update(Scene& scene) {
    using namespace Components;
    auto& ecs = scene.get_ecs();

    // Step one: Update listener position
    for (auto [trans, listener] : ecs.select<Transform, SoundListener>()) {
        auto abs_trans = make_absolute_transform(trans);
        abs_trans.position += listener.position;
        audeo::set_listener_position(abs_trans.position.x, abs_trans.position.y,
                                     abs_trans.position.z);
        audeo::set_listener_forward(listener.forward.x, listener.forward.y,
                                    listener.forward.z);
    }
    // Step 2: Update all positional audio data
    for (auto [trans, snd] : ecs.select<Transform, Sound>()) {
        auto abs_trans = make_absolute_transform(trans);
        audeo::set_position(snd.sound, abs_trans.position.x,
                            abs_trans.position.y, abs_trans.position.z);
    }
}

} // namespace Saturn::Systems
