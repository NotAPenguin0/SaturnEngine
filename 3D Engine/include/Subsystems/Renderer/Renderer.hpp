#ifndef MVG_RENDERER_HPP_
#define MVG_RENDERER_HPP_

#include "Subsystems/AssetManager/AssetManager.hpp"
#include "Subsystems/ECS/Components.hpp"
#include "Subsystems/Scene/Scene.hpp"

#include "DepthMap.hpp"
#include "Framebuffer.hpp"
#include "UniformBuffer.hpp"
#include "Utility/Utility.hpp"
#include "VertexArray.hpp"
#include "Viewport.hpp"

#include "glad/glad.h"

#include <functional>

namespace Saturn {

class Application;

class Renderer {
public:
    struct CreateInfo {
        WindowDim screen_size;
        Application& app;
    };

    Renderer(CreateInfo create_info);

    Renderer(Renderer const&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer const& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;

    ~Renderer();

    void clear(Color clear_color,
               GLenum flags = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    void render_scene(Scene& scene);

    void update_screen();

    // /brief Returns a reference to the viewport with specified index.
    // /param index: The index of the viewport to return. Viewport 0 is
    // initialized to be the full window
    Viewport& get_viewport(std::size_t index);

    // /brief Adds a viewport to the renderer
    // /param vp: The viewport to add
    // /return The index of the newly added viewport
    std::size_t add_viewport(Viewport vp);

    static constexpr std::size_t MaxLightsPerType = 15;

private:
    struct LightSizesBytes {
        static constexpr std::size_t PaddingAfterSizeVars = 4;
        static constexpr std::size_t PointLightGLSL = 4 * sizeof(glm::vec4);
        static constexpr std::size_t DirectionalLightGLSL =
            4 * sizeof(glm::vec4);
        static constexpr std::size_t SpotLightGLSL = 6 * sizeof(glm::vec4);
    };

	static constexpr std::size_t DepthMapPrecision = 1024;

    // Initialization
    void setup_framebuffer(CreateInfo const& create_info);
    void create_default_viewport(CreateInfo const& create_info);
    void initialize_postprocessing();
    void create_uniform_buffers();
    void load_default_shaders();
	void create_depth_map();

    // Rendering functions
    void render_viewport(Scene& scene, Viewport& vp);
	void render_to_depthmap(Scene& scene);
    void render_particles(Scene& scene);
	glm::mat4 get_lightspace_matrix(Scene& scene);
    void send_camera_matrices(Scene& scene,
                              Viewport& vp,
                              Components::Camera& camera);
    void send_lighting_data(Scene& scene);
    void send_model_matrix(Shader& shader,
                           Components::Transform const& relative_transform);
    void send_material_data(Shader& shader, Components::Material& material);
    void unbind_textures(Components::Material& material);

    // Utility functions
    std::vector<Components::PointLight*> collect_point_lights(Scene& scene);
    std::vector<Components::DirectionalLight*>
    collect_directional_lights(Scene& scene);
    std::vector<Components::SpotLight*> collect_spot_lights(Scene& scene);

    // Member variables
    std::reference_wrapper<Application> app;
    WindowDim screen_size;
    Framebuffer framebuf;
    ///< default constructed framebuffer means screen
    Framebuffer screen_framebuf;
    VertexArray screen;
    UniformBuffer matrix_buffer;
    UniformBuffer lights_buffer;
    UniformBuffer camera_buffer;
	DepthMap shadow_depth_map;
    Resource<Shader> no_shader_error;
    // #MaybeTODO: Move this to ParticleEmitter?
    Resource<Shader> particle_shader;
	Resource<Shader> depth_shader;
    std::vector<Viewport> viewports;
};

} // namespace Saturn

#endif
