#include <saturn/ecs/registry.hpp>

#include <saturn/serialization/default_serializers.hpp>
#include <saturn/serialization/component_serializers.hpp>

namespace saturn::ecs {

registry::registry() {
    // Create root entity. This entity is defined to have ID 0
    *entities.root() = id_generator.next();
}

entity_t registry::create_entity(entity_t parent) {
    entity_t id = id_generator.next();
    auto parent_it = entities.find(parent);
    entities.insert(parent_it, id);
    return id;
}

entity_t registry::import_entity(registry& source, entity_t other) {
    entity_t imported_root_id = 0;
    auto import_fn = [this, &source, &imported_root_id](entity_t source_entity, stl::tree<entity_t>::traverse_info info) {
        entity_t id = create_entity();
        // Set root entity id
        if (info.level == 0) {
            imported_root_id = id;
        }

        nlohmann::json j;
        serialize_from_entity(j, source, source_entity);
        deserialize_into_entity(j, *this, id);
    };

    source.entities.traverse_from(source.entities.find(other), import_fn);

    return imported_root_id;
}

stl::tree<entity_t> const& registry::get_entities() const {
    return entities;
}

} // namespace saturn::ecs