#include <saturn/ecs/systems.hpp>

namespace saturn::ecs {

void system_manager::add_system(system_update_fn system) {
    systems.push_back(system);
}

void system_manager::update_all(saturn::ecs::registry& ecs) {
    for (auto sys : systems) {
        sys(ecs);
    }
}

}