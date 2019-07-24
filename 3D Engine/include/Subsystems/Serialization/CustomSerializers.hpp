#ifndef SATURN_CUSTOM_SERIALIZERS_HPP_
#define SATURN_CUSTOM_SERIALIZERS_HPP_

#include <nlohmann/json.hpp>

namespace Saturn::Components {

struct ParticleEmitter;

void from_json(nlohmann::json const& json, ParticleEmitter& emitter);

void to_json(nlohmann::json& json, ParticleEmitter const& emitter);

}

#endif
