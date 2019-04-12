#ifndef MVG_SHADER_HPP_
#define MVG_SHADER_HPP_

#include <string_view>

namespace Saturn {

class Shader {
public:
	struct CreateInfo {
        std::string_view vtx_path;
		std::string_view frag_path;
	};

	static Shader create(CreateInfo create_info);
    static void use(Shader& shader);

    Shader() = default;
    Shader(Shader&& rhs);
    Shader& operator=(Shader&& rhs);

    unsigned int handle();

    void set_int(std::string_view name, int value);
    void set_float(std::string_view name, float value);

    // Disabled until math library is done
    /*void set_vec3(std::string_view name, glm::vec3 value);
    void set_vec4(std::string_view name, glm::vec4 value);
    void set_mat4(std::string_view name, glm::mat4 value);*/

    int location(std::string_view name);


private:
    unsigned int program;
};

} // namespace Saturn

#endif
