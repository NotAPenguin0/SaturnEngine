#include <saturn/ecs/registry.hpp>

#include <saturn/serialization/default_serializers.hpp>
#include <saturn/serialization/component_serializers.hpp>

namespace saturn::ecs {

entity_t registry::create_entity() {
    entity_t id = id_generator.next();
    entities.push_back(id);
    return id;
}

entity_t registry::import_entity(registry& source, entity_t other) {
    entity_t id = create_entity();
    entities.push_back(id);

    nlohmann::json j;
    serialize_from_entity(j, source, other);
    deserialize_into_entity(j, *this, id);

    return id;
}

stl::vector<entity_t> const& registry::get_entities() const {
    return entities;
}

} // namespace saturn::ecs