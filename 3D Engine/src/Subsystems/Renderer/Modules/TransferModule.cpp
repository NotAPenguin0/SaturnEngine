#include "Subsystems/Renderer/Modules/TransferModule.hpp"

#include "Subsystems/Renderer/RenderUtils.hpp"
#include "Subsystems/Renderer/UniformBuffer.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Saturn::RenderModules {

namespace {
struct LightSizesBytes {
    static constexpr std::size_t PaddingAfterSizeVars = 4;
    static constexpr std::size_t PointLightGLSL = 4 * sizeof(glm::vec4);
    static constexpr std::size_t DirectionalLightGLSL = 4 * sizeof(glm::vec4);
    static constexpr std::size_t SpotLightGLSL = 6 * sizeof(glm::vec4);
};

constexpr std::size_t MaxLightsPerType = 15;

} // namespace

// Data transfer should be the first module to he processed
TransferModule::TransferModule() : RenderModule(0) {}

void TransferModule::init() {
    // View + Projection matrix buffer
    UniformBuffer::CreateInfo matrix_info;
    matrix_info.binding_point = 0;
    matrix_info.dynamic = true; // View/Projection matrices can change
    matrix_info.size_in_bytes = 2 * sizeof(glm::mat4);
    matrix_buffer.assign(matrix_info);

    // Lights buffer
    UniformBuffer::CreateInfo lights_info;
    lights_info.binding_point = 1;
    lights_info.dynamic =
        false; // for now, we assume lights are mostly static #CHECK
    lights_info.size_in_bytes =
        sizeof(int) +                           // point_light_count
        sizeof(int) +                           // directional_light_count
        sizeof(int) +                           // spot_light_count
        LightSizesBytes::PaddingAfterSizeVars + // padding
        MaxLightsPerType * (LightSizesBytes::PointLightGLSL +
                            LightSizesBytes::DirectionalLightGLSL +
                            LightSizesBytes::SpotLightGLSL);
    lights_buffer.assign(lights_info);

    // Camera data buffer
    UniformBuffer::CreateInfo camera_info;
    camera_info.binding_point = 2;
    camera_info.dynamic = true; // Camera data changes
    camera_info.size_in_bytes =
        sizeof(glm::vec4); // vec3 padded to the size of a vec4 because of
                           // std140 layout
    camera_buffer.assign(camera_info);
}

void TransferModule::process(Scene& scene,
                             Viewport& viewport,
                             Framebuffer& target) {
    transfer_camera_data(scene, viewport);
    transfer_lighting_data(scene);
}

void TransferModule::transfer_camera_data(Scene& scene, Viewport& vp) {
    using namespace Components;

    auto cam_id = vp.get_camera();
    auto& camera = scene.get_ecs().get_with_id<Camera>(cam_id);

    // Transfer camera data
    auto& cam_trans = camera.entity->get_component<Transform>();
    auto projection = glm::perspective(
        glm::radians(camera.fov),
        (float)vp.dimensions().x / (float)vp.dimensions().y, 0.1f, 100.0f);

    auto view = glm::lookAt(cam_trans.position,
                            cam_trans.position + camera.front, camera.up);

    UniformBuffer::bind(matrix_buffer);
    matrix_buffer.set_mat4(projection, 0);
    matrix_buffer.set_mat4(view, sizeof(glm::mat4));

    UniformBuffer::bind(camera_buffer);
    camera_buffer.set_vec3(cam_trans.position, 0);
}

void TransferModule::transfer_lighting_data(Scene& scene) {
    using namespace Components;

    UniformBuffer::bind(lights_buffer);
    auto point_lights = collect_component_pointers<PointLight>(scene);
    lights_buffer.set_int(point_lights.size(), 0);
    for (std::size_t i = 0; i < point_lights.size(); ++i) {
        auto lightpos =
            point_lights[i]->entity->get_component<Transform>().position;
        // clang-format off
        const auto point_light_offset =
            sizeof(int) + // point_light_count
			sizeof(int) + // directional_light_count
			sizeof(int) + // spot_light_count
			LightSizesBytes::PaddingAfterSizeVars +
            i * (LightSizesBytes::PointLightGLSL); // point_lights array
        // clang-format on
        lights_buffer.set_vec3(point_lights[i]->ambient, point_light_offset);
        lights_buffer.set_vec3(point_lights[i]->diffuse,
                               point_light_offset + sizeof(glm::vec4));
        lights_buffer.set_vec3(point_lights[i]->specular,
                               point_light_offset + 2 * sizeof(glm::vec4));
        lights_buffer.set_vec3(lightpos,
                               point_light_offset + 3 * sizeof(glm::vec4));
        lights_buffer.set_float(point_lights[i]->intensity,
                                point_light_offset + 3 * sizeof(glm::vec4) +
                                    sizeof(glm::vec3));
    }

    constexpr std::size_t PointLightBaseOffset =
        (sizeof(int) + sizeof(int) + sizeof(int) +
         LightSizesBytes::PaddingAfterSizeVars +
         MaxLightsPerType * (LightSizesBytes::PointLightGLSL));

    auto directional_lights =
        collect_component_pointers<DirectionalLight>(scene);
    lights_buffer.set_int(directional_lights.size(), sizeof(int));
    for (std::size_t i = 0; i < directional_lights.size(); ++i) {
        const auto offset =
            PointLightBaseOffset + i * (LightSizesBytes::DirectionalLightGLSL);
        lights_buffer.set_vec3(directional_lights[i]->ambient, offset);
        lights_buffer.set_vec3(directional_lights[i]->diffuse,
                               offset + sizeof(glm::vec4));
        lights_buffer.set_vec3(directional_lights[i]->specular,
                               offset + 2 * sizeof(glm::vec4));
        lights_buffer.set_vec3(directional_lights[i]->direction,
                               offset + 3 * sizeof(glm::vec4));
    }

    constexpr std::size_t DirectionalLightBaseOffset =
        MaxLightsPerType * LightSizesBytes::DirectionalLightGLSL;
    constexpr std::size_t OffsetBeforeSpotLights =
        PointLightBaseOffset + DirectionalLightBaseOffset;

    auto spot_lights = collect_component_pointers<SpotLight>(scene);
    lights_buffer.set_int(spot_lights.size(), 2 * sizeof(int));
    for (std::size_t i = 0; i < spot_lights.size(); ++i) {
        const auto offset =
            OffsetBeforeSpotLights + i * LightSizesBytes::SpotLightGLSL;
        auto lightpos =
            spot_lights[i]->entity->get_component<Transform>().position;
        lights_buffer.set_vec3(spot_lights[i]->ambient, offset);
        lights_buffer.set_vec3(spot_lights[i]->diffuse,
                               offset + sizeof(glm::vec4));
        lights_buffer.set_vec3(spot_lights[i]->specular,
                               offset + 2 * sizeof(glm::vec4));
        lights_buffer.set_vec3(lightpos, offset + 3 * sizeof(glm::vec4));
        lights_buffer.set_vec3(spot_lights[i]->direction,
                               offset + 4 * sizeof(glm::vec4));
        lights_buffer.set_float(
            spot_lights[i]->intensity,
            offset + 4 * sizeof(glm::vec4) +
                sizeof(glm::vec3)); // the first float is packed against the
                                    // previous vec3
        lights_buffer.set_float(
            glm::cos(glm::radians(spot_lights[i]->inner_angle)),
            offset + 5 * sizeof(glm::vec4));
        lights_buffer.set_float(
            glm::cos(glm::radians(spot_lights[i]->outer_angle)),
            offset + 5 * sizeof(glm::vec4) + sizeof(float));
    }
}

} // namespace Saturn::RenderModules
