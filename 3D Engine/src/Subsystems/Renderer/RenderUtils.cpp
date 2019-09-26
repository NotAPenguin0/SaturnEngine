#include "Subsystems/Renderer/RenderUtils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace Saturn {

glm::mat4 get_lightspace_matrix(Scene& scene) {
    // For now, we only support one directional light for shadows
    auto dirlights =
        collect_component_pointers<Components::DirectionalLight>(scene);
    if (dirlights.empty())
        throw std::runtime_error("There must be a light"); // Temporary
    auto& light = *dirlights[0];
    static constexpr float near_plane = 0.00001f;
    static constexpr float far_plane = 100.0f;
    // Orthographic projection for light
    glm::mat4 light_projection =
        glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    static constexpr float distance = 10.0f;
    glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 0.0f) +
                          distance * glm::normalize(-light.direction);
    glm::mat4 light_view = glm::lookAt(light_pos, glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightspace_mat = light_projection * light_view;
    return lightspace_mat;
}

void send_model_matrix(Shader& shader,
                       Components::Transform const& relative_transform) {
    // Make sure to get absolute transform
    auto transform = make_absolute_transform(relative_transform);

    auto model = glm::mat4(1.0f);
    // Apply transformations
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, {glm::radians(transform.rotation.x),
                                glm::radians(transform.rotation.y),
                                glm::radians(transform.rotation.z)});
    model = glm::scale(model, transform.scale);
    // Send to shader
    Shader::bind(shader);
    shader.set_mat4(Shader::Uniforms::Model, model);
    Shader::unbind();
}

} // namespace Saturn
