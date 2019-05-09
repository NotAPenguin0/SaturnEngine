#ifndef MVG_SCENE_OBJECT_HPP_
#define MVG_SCENE_OBJECT_HPP_

#include "Shader.hpp"
#include "VertexArray.hpp" // Temporary

namespace Saturn {

class Scene;

class SceneObject {
public:
    friend class Scene;

    SceneObject(Scene* s);

    struct Debug {
        VertexArray cube;
        Shader shader;
    } debug;

    template<typename C, typename... Args>
    C& add_component(Args&&... args) {
        auto& ecs = scene->ecs;
        auto& container = ecs.get_components<C>();
        auto it = container.push_back(C{std::forward<Args>(args)...});
        it->entity = this;
        return *it;
    }

    template<typename C>
    bool has_component() {
        auto& ecs = scene->ecs;
        auto& container = ecs.get_components<C>();
        for (auto& component : container) {
            if (component.entity == this) { return true; }
        }
        return false;
    }

private:
    Scene* scene;
};

} // namespace Saturn

#endif
