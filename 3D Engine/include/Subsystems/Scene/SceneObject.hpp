#ifndef SATURN_SCENE_OBJECT_HPP_
#define SATURN_SCENE_OBJECT_HPP_

#include "Subsystems/ECS/Components.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Subsystems/Serialization/ComponentSerializers.hpp"
#include "Utility/IDGenerator.hpp"

#include <nlohmann/json.hpp>

#include <typeindex>
#include <typeinfo>

namespace Saturn {

class Scene;

class SceneObject {
public:
    friend class Scene;

    SceneObject() = default;
    SceneObject(Scene* s, SceneObject* parent = nullptr);

    ~SceneObject() { remove_all_components<COMPONENT_LIST>(); }

    template<typename C, typename... Args>
    std::size_t add_component(Args&&... args) {
        auto& ecs = scene->ecs;
        auto& container = ecs.get_components<C>();

        // Grab a new id for this component

        auto it = container.push_back(C{std::forward<Args>(args)...});
        it->entity = this;
        component_ids[typeid(C)] = it->id;

        return it->id;
    }

    template<typename C>
    bool has_component() const {
        return component_ids.find(typeid(C)) != component_ids.end();
    }

    template<typename C>
    C& get_component() {
        auto& ecs = scene->ecs;
        auto& container = ecs.get_components<C>();

        return container.get_with_id(component_ids[typeid(C)]);
    }

    template<typename C>
    C const& get_component() const {
        auto& ecs = scene->ecs;
        auto& container = ecs.get_components<C>();

        return container.get_with_id(component_ids.at(typeid(C)));
    }

    template<typename C>
    void remove_component() {
        auto& ecs = scene->ecs;
        auto& container = ecs.get_components<C>();
        std::type_index ti = typeid(C);
        container.erase_component(component_ids.at(ti));
        component_ids.erase(ti);
    }

    template<typename C, typename... Cs>
    void remove_all_components() {
        if (has_component<C>()) { remove_component<C>(); }
        if constexpr (sizeof...(Cs) != 0) remove_all_components<Cs...>();
    }

    bool has_parent() const;
    SceneObject* parent();
    SceneObject const* parent() const;
    inline void set_parent(SceneObject* new_parent) { parent_obj = new_parent; }

    inline Scene* get_scene() { return scene; }

    template<typename C>
    void serialize_component(nlohmann::json& j) const {
        // Assumes the object has this component
        nlohmann::json to_add = get_component<C>();
        j.update(to_add);
    }

    template<typename C, typename... Cs>
    void serialize_components(nlohmann::json& j) const {
        if (has_component<C>()) { serialize_component<C>(j); }
        if constexpr (sizeof...(Cs) != 0) { serialize_components<Cs...>(j); }
    }

    void serialize_to_file(std::string_view path);

    friend void from_json(nlohmann::json const& j, SceneObject& obj);

    inline void set_id(std::size_t new_id) { id = new_id; }
    inline std::size_t get_id() const { return id; }

    inline void set_parent_id(std::size_t p_id) { parent_id = p_id; }
    inline std::size_t get_parent_id() const { return parent_id; }

private:
    std::size_t id = IDGenerator<SceneObject>::none;
    std::size_t parent_id = IDGenerator<SceneObject>::none;
    Scene* scene;
    SceneObject* parent_obj;
    // Maps a component type to a component id. The id will be translated in an
    // index in the ECS class
    std::unordered_map<std::type_index, std::size_t> component_ids;
};

void to_json(nlohmann::json& j, SceneObject const& obj);
void from_json(nlohmann::json const& j, SceneObject& obj);

} // namespace Saturn

#endif
