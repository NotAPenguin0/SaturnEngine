#ifndef SATURN_BLUEPRINT_COMPONENT_HPP_
#define SATURN_BLUEPRINT_COMPONENT_HPP_

#include <saturn/ecs/entity.hpp>

#include <saturn/utility/handle.hpp>
#include <saturn/assets/model.hpp>

namespace saturn::components {

struct [[component]] Blueprint {
    ecs::entity_t blueprint;
    // If this blueprint is the root of a model, this handle is set to a valid value
    Handle<assets::Model> model = { -1 };
};

}

#endif