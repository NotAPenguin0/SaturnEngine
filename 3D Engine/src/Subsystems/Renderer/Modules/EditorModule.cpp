#include "Subsystems/Renderer/Modules/EditorModule.hpp"

#include "Subsystems/AssetManager/AssetManager.hpp"
#include "Subsystems/Renderer/RenderUtils.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Saturn::RenderModules {

EditorModule::EditorModule() : RenderModule(3) {}

void EditorModule::init() {
    collider_shader =
        AssetManager<Shader>::get_resource("config/resources/shaders/collider.sh", true);
    axis_shader =
        AssetManager<Shader>::get_resource("config/resources/shaders/axis.sh", true);
    outline_shader =
        AssetManager<Shader>::get_resource("config/resources/shaders/outline.sh", true);

    box_collider_mesh =
        AssetManager<Mesh>::get_resource("config/resources/meshes/box_collider.mesh", true);
    line_mesh = AssetManager<Mesh>::get_resource("config/resources/meshes/line.mesh", true);
}

void EditorModule::process(Scene& scene,
                           Viewport& viewport,
                           Framebuffer& target) {
    render_colliders(scene);
    render_axes();
    render_outlines(scene);
}

void EditorModule::render_colliders(Scene& scene) {
    using namespace Components;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (auto [rel_trans, collider, _] :
         scene.get_ecs().select<Transform, BoxCollider, ColliderRenderer>()) {
        auto& shader = collider_shader.get();
        auto copy = rel_trans;
        copy.position += collider.center;
        copy.scale = collider.half_widths;
        copy.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        send_model_matrix(shader, copy);
        Shader::bind(shader);
        auto& vtx_array = box_collider_mesh->get_vertices();
        VertexArray::bind(vtx_array);
        glDrawElements(GL_LINES, vtx_array.index_size(), GL_UNSIGNED_INT,
                       nullptr);
        Shader::unbind();
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void EditorModule::render_outlines(Scene& scene) {
    using namespace Components;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (auto [rel_trans, mesh, outline] :
         scene.get_ecs().select<Transform, StaticMesh, OutlineRenderer>()) {

        if (!mesh.mesh.is_loaded()) { continue; }

        auto& shader = outline_shader.get();
        send_model_matrix(shader, rel_trans);
        Shader::bind(shader);
        shader.set_vec3(Shader::Uniforms::Color, outline.color);
        auto& vtx_array = mesh.mesh->get_vertices();
        VertexArray::bind(vtx_array);
        glEnable(GL_LINE_SMOOTH);
        glDrawElements(GL_LINES, vtx_array.index_size(), GL_UNSIGNED_INT,
                       nullptr);
        glDisable(GL_LINE_SMOOTH);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void EditorModule::render_axes() {
    glm::vec3 rotations[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),  // y axis
        glm::vec3(90.0f, 0.0f, 0.0f), // z axis
        glm::vec3(0.0f, 0.0f, 90.0f)  // x axis
    };
    glm::vec3 colors[] = {
        glm::vec3(0.0f, 1.0f, 0.0f), // y axis
        glm::vec3(0.0f, 0.0f, 1.0f), // z axis
        glm::vec3(1.0f, 0.0f, 0.0f)  // x axis
    };
    auto& shader = axis_shader.get();
    auto& vertices = line_mesh->get_vertices();
    Components::Transform axis_transform;
    axis_transform.scale = glm::vec3(1.0, 100.0f, 1.0f);
    axis_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    // There are 3 axes
    for (int i = 0; i < 3; ++i) {
        axis_transform.rotation = rotations[i];
        auto model = glm::mat4(1.0f);
        // Apply transformations
        model = glm::translate(model, axis_transform.position);
        model = glm::rotate(model, {glm::radians(axis_transform.rotation.x),
                                    glm::radians(axis_transform.rotation.y),
                                    glm::radians(axis_transform.rotation.z)});
        model = glm::scale(model, axis_transform.scale);
        // Send to shader
        Shader::bind(shader);
        shader.set_mat4(Shader::Uniforms::Model, model);
        VertexArray::bind(vertices);
        auto const& color = colors[i];
        shader.set_vec3(Shader::Uniforms::Color, color);
        glDrawElements(GL_LINES, vertices.index_size(), GL_UNSIGNED_INT,
                       nullptr);
        VertexArray::unbind();
    }
}

} // namespace Saturn::RenderModules
