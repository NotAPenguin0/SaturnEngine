#include "Subsystems/AssetManager/ResourceLoaders.hpp"

#include "Subsystems/Logging/LogSystem.hpp"

#include <fstream>

namespace Saturn {

std::unique_ptr<Shader> ResourceLoader<Shader>::load(std::string const& path) {
    std::ifstream file(path);
    if (!file.good()) {
        LogSystem::write(LogSystem::Severity::Error,
                         "Failed to load Shader file at path: " + path);
        return nullptr;
    }
    Shader::CreateInfo info;

    // The shader file contains two paths: the first is to the vertex shader,
    // the second to the fragment shader

    std::string vtx, frag;
    std::getline(file, vtx);
    std::getline(file, frag);

    info.vtx_path = vtx;
    info.frag_path = frag;

    return std::make_unique<Shader>(info);
}

std::unique_ptr<Mesh> ResourceLoader<Mesh>::load(std::string const& path) {
    std::ifstream file(path);
    if (!file.good()) {
        LogSystem::write(LogSystem::Severity::Error,
                         "Failed to load Mesh file at path: " + path);
        return nullptr;
    }

    // A Mesh file looks like this:
    /*
     *attributes: [N]
     *attribute_1_name location_in_shader_1 num_components_1
     *...
     *attribute_n_name location_in_shader_n num_components_n
     *vertices: [N]
     *[List of comma separated vertex data, N vertices]
     *Indices: [N] # Leave zero to auto-generate
     *[List of comma separated indices, N indices]
     **/

	// PUT A COMMA AT THE END OF THE LIST AS WELL OR PARSING FAILS

    Mesh::CreateInfo info;

    std::string dummy;
    file >> dummy;
    assert(dummy == "attributes:" && "Syntax error: Expected /'attributs:/'");
    std::size_t num_attributes;
    file >> num_attributes;

    // We'll need this when reading the vertex data itself
    std::size_t total_vertex_components = 0;

    // Read attribute data
    info.vertices.attributes.resize(num_attributes);
    for (auto& attr : info.vertices.attributes) {
        file >> dummy; // Attribute name
        std::size_t location_in_shader;
        std::size_t num_components;
        file >> location_in_shader >> num_components;

        attr.location_in_shader = location_in_shader;
        attr.num_components = num_components;

        total_vertex_components += num_components;
    }

    // Read vertex data
    file >> dummy;
    assert(dummy == "vertices:" && "Syntax error: Expected /'vertices:/'");
    std::size_t num_vertices;
    file >> num_vertices;
    info.vertices.vertices.resize(num_vertices * total_vertex_components);
    for (auto& vtx : info.vertices.vertices) {
        file >> vtx;
        char comma;
        file >> comma;
    }

    file >> dummy;
    assert(dummy == "indices:" && "Syntax error: Expected /'indices:/'");
    std::size_t num_indices;
    file >> num_indices;
    if (num_indices != 0) {
        info.vertices.indices.resize(num_indices);
        for (auto& idx : info.vertices.indices) {
            file >> idx;
            char comma;
            file >> comma;
        }
    }

    info.vertices.dynamic = false;

    return std::make_unique<Mesh>(info);
}

} // namespace Saturn
