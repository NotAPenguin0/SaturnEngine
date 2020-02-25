#include <saturn/ecs/registry.hpp>

namespace saturn::ecs {

entity_t registry::create_entity() {
    entity_t id = id_generator.next();
    entities.push_back(id);
    return id;
}



} // namespace saturn::ecs