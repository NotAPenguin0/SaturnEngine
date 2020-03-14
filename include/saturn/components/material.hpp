#ifndef SATURN_COMPONENT_MATERIAL_HPP_
#define SATURN_COMPONENT_MATERIAL_HPP_

#include <phobos/forward.hpp>
#include <saturn/utility/handle.hpp>

namespace saturn::components {

struct [[component]] Material {
    Handle<ph::Texture> texture;
};

}

#endif