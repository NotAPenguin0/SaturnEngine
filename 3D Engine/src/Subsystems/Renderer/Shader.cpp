#include "Subsystems/Renderer/Shader.hpp"

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

#include "Subsystems/Logging/LogSystem.hpp"
#include "Subsystems/Renderer/OpenGL.hpp"

#include "Editor/EditorLog.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace Saturn {

static unsigned int create_shader(const char* vtx_path,
                                  const char* frag_path,
                                  const char* geom_path = nullptr) {
    using namespace std::literals::string_literals;

    std::fstream file(vtx_path);

    if (!file.good()) {
        log::error(fmt::format("[SHADER::VERTEX]: failed to open vertex shader "
                               "source file at path {}",
                               vtx_path));
        return 0;
    }

    std::stringstream buf;
    buf << file.rdbuf();

    std::string vtx_source(buf.str());

    file.close();
    buf = std::stringstream{}; // reset buffer
    file.open(frag_path);

    if (!file.good()) {
        log::error(fmt::format(
            "[SHADER::FRAGMENT]: failed to open fragment shader source file at "
            "path {}",
            frag_path));
        return 0;
    }

    buf << file.rdbuf();

    std::string frag_source(buf.str());
    buf = std::stringstream{};

    std::string geom_source;
    file.close();
    if (geom_path != nullptr) {
        file.open(geom_path);
        if (!file.good()) {
            log::error(fmt::format("[SHADER::GEOMETRY]: failed to open "
                                   "geometry shader source file at path {}",
                                   frag_path));
            return 0;
        }
        buf << file.rdbuf();
        geom_source = buf.str();
    }

    unsigned int vtx_shader, frag_shader, geom_shader = 0;
    vtx_shader = glCreateShader(GL_VERTEX_SHADER);
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (geom_source != "") {
        geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
        const char* geom_carr = geom_source.c_str();
        glShaderSource(geom_shader, 1, &geom_carr, nullptr);
        glCompileShader(geom_shader);
        int success;
        char infolog[512];
        glGetShaderiv(geom_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(geom_shader, 512, nullptr, infolog);
            log::error(fmt::format(
                "Failed to compile geometry shader at path: {}", vtx_path));
            log::error(fmt::format("[SHADER::GEOMETRY::COMPILATION_FAILED]: {}",
                                   infolog));
            throw "";
            return 0;
        }
    }
    // This is wrapped inside a lambda to limit the scope of vtx_carr and
    // frag_carr
    [&vtx_source, &frag_source, &vtx_shader, &frag_shader]() {
        const char* vtx_carr = vtx_source.c_str();
        const char* frag_carr = frag_source.c_str();
        glShaderSource(vtx_shader, 1, &vtx_carr, nullptr);
        glShaderSource(frag_shader, 1, &frag_carr, nullptr);
    }();

    glCompileShader(vtx_shader);
    glCompileShader(frag_shader);

    // Now, check for compilation errors
    int success;
    char infolog[512];
    glGetShaderiv(vtx_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vtx_shader, 512, nullptr, infolog);
        log::error(fmt::format("Failed to compile vertex shader at path: {}",
                               vtx_path));
        log::error(
            fmt::format("[SHADER::VERTEX::COMPILATION_FAILED]: {}", infolog));
        throw "";
        return 0;
    }

    // And again for the fragment shader
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag_shader, 512, nullptr, infolog);
        log::error(fmt::format("Failed to compile fragment shader at path: {}",
                               vtx_path));
        log::error(
            fmt::format("[SHADER::FRAGMENT::COMPILATION_FAILED]: {}", infolog));
        throw ""; //#UGLYANDMUSTREMOVELATER
        return 0;
    }

    // Finally, link the vertex and fragment shader together
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vtx_shader);
    glAttachShader(shaderProgram, frag_shader);
    if (geom_shader != 0) { glAttachShader(shaderProgram, geom_shader); }
    glLinkProgram(shaderProgram);

    // Check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);
        log::error(
            fmt::format("Failed to link shader. Vertex\n: {}\nFragment: {}",
                        vtx_path, frag_path));
        log::error(fmt::format("[SHADER::LINK_FAILED]: ", infolog));
        throw "";
        return 0;
    }

    // These are linked now and can safely be deleted
    glDeleteShader(vtx_shader);
    glDeleteShader(frag_shader);
    if (geom_shader != 0) { glDeleteShader(geom_shader); }

    return shaderProgram;
}

Shader::Shader(CreateInfo create_info) { assign(create_info); }

void Shader::assign(CreateInfo create_info) {
    if (program != 0) { glDeleteProgram(program); }
    program =
        create_shader(create_info.vtx_path.data(), create_info.frag_path.data(),
                      create_info.geom_path.data());
	if (!program) {
		std::cout << create_info.vtx_path << " " << create_info.frag_path << "\n";
	}
}

unsigned int Shader::handle() { return program; }

void Shader::set_int(std::string_view name, int value) {
    glUniform1i(location(name), value);
}

void Shader::set_float(std::string_view name, float value) {
    glUniform1f(location(name), value);
}

void Shader::set_vec3(std::string_view name, glm::vec3 const& value) {
    glUniform3fv(location(name), 1, glm::value_ptr(value));
}

void Shader::set_vec4(std::string_view name, glm::vec4 const& value) {
    glUniform4fv(location(name), 1, glm::value_ptr(value));
}

void Shader::set_mat4(std::string_view name, glm::mat4 const& value) {
    glUniformMatrix4fv(location(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_int(int loc, int value) {
    assert(loc != -1);
    glUniform1i(loc, value);
}

void Shader::set_float(int loc, float value) {
    assert(loc != -1);
    glUniform1f(loc, value);
}

void Shader::set_vec3(int loc, glm::vec3 const& value) {
    assert(loc != -1);
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::set_vec4(int loc, glm::vec4 const& value) {
    assert(loc != -1);
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void Shader::set_mat4(int loc, glm::mat4 const& value) {
    assert(loc != -1);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

int Shader::location(std::string_view name) {
    auto& loc_data = uniform_cache[name];
    if (loc_data != 0) { return loc_data; }

    loc_data = glGetUniformLocation(program, name.data());
    assert(loc_data != -1);

    return loc_data;
}

void Shader::bind(Shader& shader) { glUseProgram(shader.program); }
void Shader::unbind() { glUseProgram(0); }

} // namespace Saturn
