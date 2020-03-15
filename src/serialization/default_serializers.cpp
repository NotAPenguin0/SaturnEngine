#include <saturn/serialization/default_serializers.hpp>
#include <saturn/assets/assets.hpp>

static saturn::Context* serialize_context;

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

} // namespace glm

namespace saturn {

void set_serialize_context(Context* ctx) {
    serialize_context = ctx;
}

void from_json(nlohmann::json const& j, Handle<ph::Mesh>& handle) {
    handle = assets::load_mesh(*serialize_context, j.get<std::string>());
}

void to_json(nlohmann::json& j, Handle<ph::Mesh> const& handle) {
    j = assets::get_mesh_path(handle).generic_string();
}

void from_json(nlohmann::json const& j, Handle<ph::Texture>& handle) {
    if (j.is_null()) { 
        handle.id = -1; 
    } else {
        handle = assets::load_texture(*serialize_context, j.get<std::string>());
    }
}

void to_json(nlohmann::json& j, Handle<ph::Texture> const& handle) {
    if (handle.id == -1) { return; }
    j = assets::get_texture_path(handle).generic_string();
}


void from_json(nlohmann::json const& j, Handle<ph::Material>& handle) {
    handle.id = j.get<stl::int64_t>();
}

void to_json(nlohmann::json& j, Handle<ph::Material> const& handle) {
    j = handle.id;
}

} // namespace saturn