#ifndef SATURN_DEFAULT_SERIALIZERS_HPP_
#define SATURN_DEFAULT_SERIALIZERS_HPP_

#include <glm/vec3.hpp>
#include <phobos/assets/handle.hpp>
#include <nlohmann/json.hpp>

namespace glm {

void from_json(nlohmann::json const& j, vec3& v);
void to_json(nlohmann::json& j, vec3 const& v);

}

namespace ph {

template<typename T>
void from_json(nlohmann::json const& j, Handle<T>& handle) {
    // Not implemented
}

template<typename T>
void to_json(nlohmann::json& j, Handle<T> const& handle) {
    // Not implemented
}

}

#endif