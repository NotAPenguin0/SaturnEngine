#ifndef SATURN_ASSETS_MODEL_HPP_
#define SATURN_ASSETS_MODEL_HPP_

#include <saturn/ecs/entity.hpp>

namespace saturn::assets {

// A model is simply an entity in the blueprints ecs
class Model { 
public:
    ecs::entity_t blueprint; 
};

}

#endif