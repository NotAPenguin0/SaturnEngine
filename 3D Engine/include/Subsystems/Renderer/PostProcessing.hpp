#ifndef SATURN_POST_PROCESSING_HPP_
#define SATURN_POST_PROCESSING_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Shader.hpp"

namespace Saturn {

class PostProcessing {
public:
    static constexpr const char* vertex_path =
        "config/resources/shaders/postprocessing/default_v.glsl";

    void load_shaders(const char* fname);

    void add_shader(std::string const& name, Resource<Shader> shader);

    Resource<Shader> get(std::string const& name);

    void set_active(std::string const& name);
    Resource<Shader> get_active();

    static PostProcessing& get_instance() {
        static std::unique_ptr<PostProcessing> instance;
        if (instance == nullptr) {
            instance = std::make_unique<PostProcessing>();
        }
        return *instance;
    }

private:
    std::unordered_map<std::string, Resource<Shader>> shaders;
    Resource<Shader> active;
};

} // namespace Saturn

#endif
