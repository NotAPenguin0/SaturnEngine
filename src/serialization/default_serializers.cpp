#include <saturn/serialization/default_serializers.hpp>

namespace glm {

void from_json(nlohmann::json const& j, vec3& v) {
    v.x = j[0];
    v.y = j[1];
    v.z = j[2];
}

void to_json(nlohmann::json& j, vec3 const& v) {
    j[0] = v.x;
    j[1] = v.y;
    j[2] = v.z;
}

}