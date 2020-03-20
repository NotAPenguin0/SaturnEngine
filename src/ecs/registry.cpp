#include <saturn/ecs/registry.hpp>

#include <saturn/serialization/default_serializers.hpp>
#include <saturn/serialization/component_serializers.hpp>
#include <saturn/meta/for_each_component.hpp>

#include <saturn/components/blueprint.hpp>
#include <saturn/components/blueprint_instance.hpp>

#include <fstream>

#include <stl/tuple.hpp>

namespace saturn::ecs {

namespace {

template<typename C>
struct component_copy {
    void operator()(registry const& src, registry& dst, entity_t src_entity, entity_t dst_entity) {
        // Do not copy blueprint component
        if constexpr (std::is_same_v<C, components::Blueprint>) { return; }
        // Do the copy
        if (src.has_component<C>(src_entity)) {
            dst.add_component<C>(dst_entity, src.get_component<C>(src_entity));
        }
    }
};

}

registry::registry() {

}

entity_t registry::create_entity(entity_t parent) {
    entity_t id = id_generator.next();
    auto parent_it = entities.find(parent);
    entities.insert(parent_it, id);
    return id;
}

entity_t registry::create_blueprint_entity(entity_t parent) {
    entity_t entity = create_entity(parent);
    add_component<components::Blueprint>(entity, entity);
    return entity;
}

entity_t registry::import_blueprint(registry& source, entity_t other) {
    auto copy_fun = [this, &source](entity_t src_entity, stl::tree<entity_t>::const_traverse_info info, entity_t parent) 
        -> stl::tuple<entity_t> {
        
        auto id = create_entity(parent);
        // do the copy
        meta::for_each_component<component_copy>(source, *this, src_entity, id);
        // Add a blueprint instance component referring to the old entity (= the blueprint)
        add_component<components::BlueprintInstance>(id, src_entity);

        return { id };
    };

    auto entity_it = source.get_entities().find(other);
    source.get_entities().traverse_from(entity_it, copy_fun, 0);

    // Return the latest created child of the root entity, aka the root of the entity that was just created
    return entities.root()->children.back().data;
}

stl::tree<entity_t> const& registry::get_entities() const {
    return entities;
}

} // namespace saturn::ecs