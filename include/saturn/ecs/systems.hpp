#ifndef SATURN_ECS_SYSTEMS_HPP_
#define SATURN_ECS_SYSTEMS_HPP_

#include <saturn/ecs/registry.hpp>
#include <stl/vector.hpp>

namespace saturn::ecs {

using system_update_fn = void(*)(saturn::ecs::registry&);

class system_manager {
public:
    void add_system(system_update_fn system);
    void update_all(saturn::ecs::registry& ecs);

private:
    stl::vector<system_update_fn> systems;
};

}

#endif