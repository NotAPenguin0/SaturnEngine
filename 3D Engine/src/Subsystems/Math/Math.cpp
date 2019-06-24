#include "Subsystems/Math/Math.hpp"

namespace glm {

void to_json(nlohmann::json& j, vec2 const& v) {
    j = nlohmann::json::array({v.x, v.y});
}

void to_json(nlohmann::json& j, vec3 const& v) {
    j = nlohmann::json::array({v.x, v.y, v.z});
}

void to_json(nlohmann::json& j, vec4 const& v) {
    j = nlohmann::json::array({v.x, v.y, v.z, v.w});
}

void from_json(nlohmann::json const& j, vec2& v) {
    if (!j.is_array()) {
        throw std::runtime_error("Invalid json for vec3. Must be array type");
    }
    v.x = j[0];
    v.y = j[1];
}

void from_json(nlohmann::json const& j, vec3& v) {
    if (!j.is_array()) {
        throw std::runtime_error("Invalid json for vec3. Must be array type");
    }
    v.x = j[0];
    v.y = j[1];
    v.z = j[2];
}

void from_json(nlohmann::json const& j, vec4& v) {
    if (!j.is_array()) {
        throw std::runtime_error("Invalid json for vec3. Must be array type");
    }
    v.x = j[0];
    v.y = j[1];
    v.z = j[2];
    v.w = j[3];
}

} // namespace glm
