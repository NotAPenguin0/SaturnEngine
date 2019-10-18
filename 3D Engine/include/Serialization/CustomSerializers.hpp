#ifndef SATURN_CUSTOM_SERIALIZERS_HPP_
#define SATURN_CUSTOM_SERIALIZERS_HPP_

#include <nlohmann/json.hpp>

#include "ECS/Components/DoNotSerialize.hpp"
#include "ECS/Components/Sound.hpp"

#include "Utility/UIAnchor.hpp"

namespace Saturn::Components {

struct ParticleEmitter;

void from_json(nlohmann::json const& json, ParticleEmitter& emitter);
inline void from_json(nlohmann::json const&, DoNotSerialize&) {}
inline void from_json(nlohmann::json const&, Sound&) {}

void to_json(nlohmann::json& json, ParticleEmitter const& emitter);
inline void to_json(nlohmann::json&, DoNotSerialize const&) {}
inline void to_json(nlohmann::json&, Sound const&) {}

} // namespace Saturn::Components

namespace Saturn::ui_anchors {
inline void from_json(nlohmann::json const& json, anchor_t& anchor) {
    anchor.id = json.get<size_t>();
}

inline void to_json(nlohmann::json& json, anchor_t const& anchor) {
    json = anchor.id;
}
} // namespace Saturn::ui_anchors

#endif
