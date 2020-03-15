#ifndef SATURN_DEFAULT_SERIALIZERS_HPP_
#define SATURN_DEFAULT_SERIALIZERS_HPP_

#include <glm/vec3.hpp>
#include <phobos/forward.hpp>
#include <nlohmann/json.hpp>

#include <saturn/utility/handle.hpp>
#include <saturn/utility/context.hpp>

namespace glm {

void from_json(nlohmann::json const& j, vec3& v);
void to_json(nlohmann::json& j, vec3 const& v);

}

namespace saturn {

void set_serialize_context(Context* ctx);

void from_json(nlohmann::json const& j, Handle<ph::Mesh>& handle);
void to_json(nlohmann::json& j, Handle<ph::Mesh> const& handle);

void from_json(nlohmann::json const& j, Handle<ph::Texture>& handle);
void to_json(nlohmann::json& j, Handle<ph::Texture> const& handle);

void from_json(nlohmann::json const& j, Handle<ph::Material>& handle);
void to_json(nlohmann::json& j, Handle<ph::Material> const& handle);

}

#endif