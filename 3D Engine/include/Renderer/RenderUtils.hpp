#ifndef SATURN_RENDER_UTILS_HPP_
#define SATURN_RENDER_UTILS_HPP_

#include "Renderer/Shader.hpp"
#include "Scene/Scene.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace Saturn {

glm::mat4 get_lightspace_matrix(Scene& scene);

void send_model_matrix(Shader& shader,
                       Components::Transform const& relative_transform);

template<typename C>
std::vector<C*> collect_component_pointers(Scene& scene) {
    std::vector<C*> result;
    for (auto [c] : scene.get_ecs().select<C>()) { result.push_back(&c); }
    return result;
}

} // namespace Saturn

#endif
