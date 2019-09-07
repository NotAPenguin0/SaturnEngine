#ifndef SATURN_CUSTOM_SERIALIZERS_HPP_
#define SATURN_CUSTOM_SERIALIZERS_HPP_

#include <nlohmann/json.hpp>

#include "Subsystems/ECS/Components/DoNotSerialize.hpp"

namespace Saturn::Components {

struct ParticleEmitter;

void from_json(nlohmann::json const& json, ParticleEmitter& emitter);
inline void from_json(nlohmann::json const&, DoNotSerialize&) {}

void to_json(nlohmann::json& json, ParticleEmitter const& emitter);
inline void to_json(nlohmann::json&, DoNotSerialize const&) {}
}

#endif
