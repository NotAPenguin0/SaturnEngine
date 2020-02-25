#ifndef SATURN_COMPONENT_MATERIAL_HPP_
#define SATURN_COMPONENT_MATERIAL_HPP_

#include <phobos/renderer/material.hpp>
#include <phobos/assets/handle.hpp>

namespace saturn::components {

struct Material {
    ph::Handle<ph::Texture> texture;
};

}

#endif