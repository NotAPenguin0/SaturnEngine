#include <saturn/ecs/registry.hpp>

#include <saturn/serialization/default_serializers.hpp>
#include <saturn/serialization/component_serializers.hpp>

#include <fstream>

namespace saturn::ecs {

registry::registry() {

}

entity_t registry::create_entity(entity_t parent) {
    entity_t id = id_generator.next();
    auto parent_it = entities.find(parent);
    entities.insert(parent_it, id);
    return id;
}

entity_t registry::import_entity(registry& source, entity_t other) {
    // Serializing now automatically handles children
    entity_t id = create_entity();
    nlohmann::json j;
    serialize_from_entity(j, source, other);
    deserialize_into_entity(j, *this, id);

    return id;
}

stl::tree<entity_t> const& registry::get_entities() const {
    return entities;
}

} // namespace saturn::ecs