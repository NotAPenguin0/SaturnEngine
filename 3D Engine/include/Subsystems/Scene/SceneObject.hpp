#ifndef MVG_SCENE_OBJECT_HPP_
#define MVG_SCENE_OBJECT_HPP_

#include "Subsystems\AssetManager\Resource.hpp"
#include "Subsystems\Renderer\Shader.hpp"
#include "Utility\IDGenerator.hpp"

#include <typeindex>
#include <typeinfo>

namespace Saturn {

class Scene;

class SceneObject {
public:
    friend class Scene;

    SceneObject(Scene* s);

    struct Debug {
        Resource<Shader> shader;
    } debug;

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
    bool has_component() {
        return component_ids.find(typeid(C)) != component_ids.end();
    }

    template<typename C>
    C& get_component() {
        auto& ecs = scene->ecs;
        auto& container = ecs.get_components<C>();

        return container.get_with_id(component_ids[typeid(C)]);
    }

    template<typename C>
    void remove_component() {
        auto& ecs = scene->ecs;
        auto& container = ecs.get_components<C>();
        std::type_index ti = typeid(C);
        container.erase_component(component_ids.at(ti));
        component_ids.erase(ti);
    }

private:
    Scene* scene;
    // Maps a component type to a component id. The id will be translated in an
    // index in the ECS class
    std::unordered_map<std::type_index, std::size_t> component_ids;
};

} // namespace Saturn

#endif
