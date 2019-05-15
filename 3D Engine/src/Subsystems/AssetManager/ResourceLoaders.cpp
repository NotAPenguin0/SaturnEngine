#include "Subsystems\AssetManager\ResourceLoaders.hpp"

#include <fstream>

namespace Saturn {

std::unique_ptr<Shader> ResourceLoader<Shader>::load(std::string const& path) {
    std::ifstream file(path);
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

} // namespace Saturn
