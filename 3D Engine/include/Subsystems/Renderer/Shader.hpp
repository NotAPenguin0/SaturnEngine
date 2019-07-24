#ifndef SATURN_SHADER_HPP_
#define SATURN_SHADER_HPP_

#include <string_view>
#include <unordered_map>

#include "Subsystems/Math/Math.hpp"

namespace Saturn {

class Shader {
public:
    struct CreateInfo {
        std::string_view vtx_path;
        std::string_view frag_path;
    };

    static void bind(Shader& shader);
    static void unbind();

    Shader() = default;
    Shader(CreateInfo create_info);
    Shader(Shader&& rhs) = delete;
    Shader& operator=(Shader&& rhs) = delete;

    void assign(CreateInfo create_info);

    unsigned int handle();

    void set_int(std::string_view name, int value);
    void set_float(std::string_view name, float value);

    void set_vec3(std::string_view name, glm::vec3 const& value);
    void set_vec4(std::string_view name, glm::vec4 const& value);
    void set_mat4(std::string_view name, glm::mat4 const& value);

    void set_int(int loc, int value);
    void set_float(int loc, float value);

    void set_vec3(int loc, glm::vec3 const& value);
    void set_vec4(int loc, glm::vec4 const& value);
    void set_mat4(int loc, glm::mat4 const& value);

    int location(std::string_view name);

    struct Uniforms {
        static constexpr int Model = 2;
        static constexpr int Color = 4;
        static constexpr int Scale = 3;
        static constexpr int Texture = 5;
        static constexpr int Position = 2;

        struct Material {
            static constexpr int DiffuseMap = 6;
            static constexpr int SpecularMap = 7;
			static constexpr int Shininess = 8;
        };
		
		static constexpr int LightSpaceMatrix = 9;
		static constexpr int DepthMap = 10;
    };

private:
    unsigned int program = 0;
    std::unordered_map<std::string_view, int> uniform_cache;
};

} // namespace Saturn

#endif
