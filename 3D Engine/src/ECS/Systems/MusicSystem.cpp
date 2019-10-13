#include "ECS/Systems/MusicSystem.hpp"

#include "Input/Input.hpp"
#include "Scene/Scene.hpp"

#include "ECS/Components/MusicController.hpp"
#include "ECS/Components/Sound.hpp"

#include <audeo/audeo.hpp>

#include "Editor/EditorLog.hpp"

// This system is part of a user script. It is not part of the main engine.

namespace Saturn::Systems {

using namespace Components;

void MusicSystem::on_start(Scene& scene) {
    for (auto [controller] : scene.get_ecs().select<MusicController>()) {
        audeo::Sound sound;
        if (controller.loop) {
            sound =
                audeo::play_sound(controller.music.get(), audeo::loop_forever,
                                  controller.fade_in_ms);
        } else {
            sound = audeo::play_sound(controller.music.get(), 0,
                                      controller.fade_in_ms);
        }
        auto& sound_comp = scene.get_ecs().get_with_id<Sound>(
            controller.entity->add_component<Sound>());
        sound_comp.sound = sound;

        controller.is_playing = true;
    }
}

void MusicSystem::on_update(Scene& scene) {
    for (auto [controller, sound] :
         scene.get_ecs().select<MusicController, Sound>()) {

        // Remove sound component if it has finished playing
        if (!audeo::is_valid(sound.sound)) {
            controller.entity->remove_component<Sound>();
            controller.is_playing = false;
        }

        // Toggle music if M key is pressed
        if (RawInput::get_key(Key::M).down &&
            RawInput::get_key(Key::M).has_changed) {

            if (controller.is_playing) {
                audeo::pause_sound(sound.sound);
                controller.is_playing = false;
            } else {
                audeo::resume_sound(sound.sound);
                controller.is_playing = true;
            }
        }
    }
}

void MusicSystem::on_exit(Scene& scene) {
    for (auto [controller, snd] :
         scene.get_ecs().select<MusicController, Sound>()) {
        audeo::stop_sound(snd.sound);
        controller.entity->remove_component<Sound>();
        controller.is_playing = false;
    }
}

} // namespace Saturn::Systems
