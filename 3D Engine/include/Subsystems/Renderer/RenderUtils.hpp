#ifndef SATURN_RENDER_UTILS_HPP_
#define SATURN_RENDER_UTILS_HPP_

#include "Subsystems/Renderer/Shader.hpp"
#include "Subsystems/Scene/Scene.hpp"

#include <glm/glm.hpp>

namespace Saturn {

glm::mat4 get_lightspace_matrix(Scene& scene);

void send_model_matrix(Shader& shader,
                       Components::Transform const& relative_transform);

} // namespace Saturn

#endif
