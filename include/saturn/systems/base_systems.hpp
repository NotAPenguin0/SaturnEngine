#ifndef SATURN_BASE_SYSTEMS_HPP_
#define SATURN_BASE_SYSTEMS_HPP_

#include <saturn/ecs/systems.hpp>

namespace saturn::systems {

void register_base_systems(saturn::ecs::system_manager& mngr);

namespace test_scale_system {
    void update(saturn::ecs::registry& ecs);
}

}

#endif