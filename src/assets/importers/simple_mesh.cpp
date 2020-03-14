#include <saturn/assets/importers/simple_mesh.hpp>

#include <phobos/renderer/mesh.hpp>

#include <fstream>
#include <stl/types.hpp>
#include <stl/vector.hpp>

#include <numeric>

namespace saturn::assets::importers {
    
ph::Mesh import_simple_mesh(Context& ctx, fs::path const& path) {
    std::ifstream file(path);
    if (!file.good()) {
        throw std::runtime_error("file " + path.generic_string() + " not found.");
    }
    
    stl::size_t total_values = 0;
    stl::size_t vertex_count = 0;

    file >> total_values >> vertex_count;

    stl::size_t const values_per_vertex = total_values / vertex_count;
    stl::vector<float> vertices(stl::tags::uninitialized, total_values);

    for (stl::size_t i = 0; i < total_values; ++i) {
        file >> vertices[i];
    }

    stl::vector<stl::uint32_t> indices(stl::tags::uninitialized, vertex_count);
    std::iota(indices.begin(), indices.end(), 0);

    ph::Mesh::CreateInfo info;
    info.ctx = ctx.vulkan;
    info.index_count = vertex_count;
    info.vertex_size = values_per_vertex;
    info.vertices = vertices.data();
    info.vertex_count = vertex_count;
    info.indices = indices.data();
    info.index_count = vertex_count;
    return ph::Mesh(info);
}

}