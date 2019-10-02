#include "Subsystems/Renderer/PostProcessing.hpp"

#include "Subsystems/AssetManager/AssetManager.hpp"

#include <fstream>

namespace Saturn {

void PostProcessing::load_shaders(const char* fname) {
    std::ifstream file(fname);
    if (!file.good())
        throw std::runtime_error(std::string("Failed to open file at path ") +
                                 fname);

    /*A PostProcessingShader file contains of entries like:
      [effect_name] path_to_fragment_shader
      [effect_name2] path_to_other_fragment_shader
      ...etch
    */

    std::string name;
    while (file >> name) {
        std::string frag_path;
        file >> frag_path;
        std::string outname =
            "resources/shaders/postprocessing/" + name + ".sh";
        std::ofstream out(outname);
        out << vertex_path << "\n" << frag_path;
        out.close();
        shaders[name] = AssetManager<Shader>::get_resource(outname, true);
    }
}

void PostProcessing::add_shader(std::string const& name,
                                Resource<Shader> shader) {
    shaders[std::string(name)] = shader;
}

Resource<Shader> PostProcessing::get(std::string const& name) {
    return shaders.at(name);
}

void PostProcessing::set_active(std::string const& name) {
    active = shaders.at(name);
}

Resource<Shader> PostProcessing::get_active() { return active; }

} // namespace Saturn
