#include "Subsystems\Scene\SceneObject.hpp"
#include "Subsystems\Scene\Scene.hpp"

#include <vector>

namespace Saturn {

static std::vector<float> cube_vertices = {
    -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,
    1.0f,  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,

    1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,

    -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,
    1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,

    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f};

static std::vector<unsigned int> cube_indices = {};

SceneObject::SceneObject(Scene* s) : scene(s) {
    // Debug:
    Shader::CreateInfo debug_create_info{"resources/shaders/default_v.glsl",
                                         "resources/shaders/default_f.glsl"};
    debug.shader.assign(debug_create_info);
    std::vector<VertexAttribute> attributes;
    attributes.push_back({0, 3}); // Position
    debug.cube.assign({attributes, cube_vertices, cube_indices});
}

} // namespace Saturn
