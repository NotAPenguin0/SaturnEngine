#ifndef SATURN_EDITOR_DISPLAY_FIELD_HPP_
#define SATURN_EDITOR_DISPLAY_FIELD_HPP_

#include <string_view>

#include <glm/vec3.hpp>

#include <saturn/utility/handle.hpp>
#include <saturn/utility/color.hpp>

#include <saturn/assets/assets.hpp>

namespace editor {

struct display_field {
public:
    void operator()(std::string_view name, float* value);
    void operator()(std::string_view name, glm::vec3* value);
    void operator()(std::string_view name, saturn::color3* value);

    void operator()(std::string_view name, saturn::Handle<ph::Mesh>* value);
    void operator()(std::string_view name, saturn::Handle<ph::Texture>* value);
    void operator()(std::string_view name, saturn::Handle<ph::Material>* value);
    void operator()(std::string_view name, saturn::Handle<saturn::assets::Model>* value);
};

}

#endif