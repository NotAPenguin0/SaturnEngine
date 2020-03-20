#ifndef SATURN_COMPONENTS_BLUEPRINT_INSTANCE_HPP_
#define SATURN_COMPONENTS_BLUEPRINT_INSTANCE_HPP_

#include <saturn/ecs/entity.hpp>

namespace saturn::components {

struct [[component]] BlueprintInstance {
    ecs::entity_t blueprint;
};

}

#endif