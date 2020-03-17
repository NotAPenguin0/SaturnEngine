#include <editor/widgets/display_field.hpp>

#include <imgui/imgui.h>
#include <fmt/format.h>

namespace editor {

static std::string_view remove_component_tag(std::string_view name) {
    stl::size_t pos = name.find_first_of('#');
    return name.substr(0, pos);
}

void display_field::operator()(std::string_view name, float* value) {
    ImGui::DragFloat(name.data(), value);
}

void display_field::operator()(std::string_view name, glm::vec3* value) {
    ImGui::DragFloat3(name.data(), &value->x);
}

void display_field::operator()(std::string_view name, saturn::color3* value) {
    ImGui::ColorEdit3(name.data(), &value->r);
}

void display_field::operator()(std::string_view name, saturn::Handle<ph::Mesh>* value) {
    ImGui::TextUnformatted(
        fmt::format("{}: {}", remove_component_tag(name), saturn::assets::get_mesh_path(*value).generic_string()).c_str()
    );
}

void display_field::operator()(std::string_view name, saturn::Handle<ph::Texture>* value) {
    ImGui::TextUnformatted(
        fmt::format("{}: {}", remove_component_tag(name), saturn::assets::get_texture_path(*value).generic_string()).c_str()
    );
}

void display_field::operator()(std::string_view name, saturn::Handle<ph::Material>* value) {
    ImGui::TextUnformatted(
        fmt::format("{}: {}", remove_component_tag(name), saturn::assets::get_material_path(*value).generic_string()).c_str()
    );
}

void display_field::operator()(std::string_view name, saturn::Handle<saturn::assets::Model>* value) {
    ImGui::TextUnformatted(
        fmt::format("{}: {}", remove_component_tag(name), saturn::assets::get_model_path(*value).generic_string()).c_str()
    );
}

} // namespace editor