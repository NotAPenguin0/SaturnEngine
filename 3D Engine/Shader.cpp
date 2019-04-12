#include "Shader.hpp"

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

#include "LogSystem.hpp"
#include "OpenGL.hpp"

namespace Saturn {

static unsigned int create_shader(const char* vtx_path, const char* frag_path) {
    using namespace std::literals::string_literals;

    std::fstream file(vtx_path);

    if (!file.good()) {
        LogSystem::write(
            LogSystem::Severity::Error,
            "[SHADER::VERTEX]: failed to open vertex shader source file at path"s +
                vtx_path);
        return 0;
    }

    std::stringstream buf;
    buf << file.rdbuf();

    std::string vtx_source(buf.str());

    file.close();
    buf = std::stringstream{}; // reset buffer
    file.open(frag_path);

    if (!file.good()) {
        LogSystem::write(
            LogSystem::Severity::Error,
            "[SHADER::FRAGMENT]: failed to open fragment shader source file at path"s +
                frag_path);
        return 0;
    }

    buf << file.rdbuf();

    std::string frag_source(buf.str());

    unsigned int vtx_shader, frag_shader;
    vtx_shader = glCreateShader(GL_VERTEX_SHADER);
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
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
        LogSystem::write(LogSystem::Severity::Error,
                         "Failed to compile vertex shader at path: "s +
                             vtx_path);
        LogSystem::write(LogSystem::Severity::Error,
                         "[SHADER::VERTEX::COMPILATION_FAILED]: "s + infolog);
        return 0;
    }

    // And again for the fragment shader
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag_shader, 512, nullptr, infolog);
        LogSystem::write(LogSystem::Severity::Error,
                         "Failed to compile fragment shader at path: "s +
                             frag_path);
        LogSystem::write(LogSystem::Severity::Error,
                         "[SHADER::FRAGMENT::COMPILATION_FAILED]: "s + infolog);
        return 0;
    }

    // Finally, link the vertex and fragment shader together
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vtx_shader);
    glAttachShader(shaderProgram, frag_shader);
    glLinkProgram(shaderProgram);

    // Check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);
        LogSystem::write(LogSystem::Severity::Error,
                         "Failed to link shader. Vertex\n: "s + vtx_path +
                             "\nFragment: "s + frag_path);
        LogSystem::write(LogSystem::Severity::Error,
                         "[SHADER::LINK_FAILED]: "s + infolog);
        return 0;
    }

    // These are linked now and can safely be deleted
    glDeleteShader(vtx_shader);
    glDeleteShader(frag_shader);

    return shaderProgram;
}

Shader Shader::create(CreateInfo create_info) {
	Shader s;
    s.program = create_shader(create_info.vtx_path.data(), create_info.frag_path.data());
    return s;
}

Shader::Shader(Shader&& rhs) {
    program = rhs.program;
    rhs.program = 0;
}

Shader& Shader::operator=(Shader&& rhs) {
    program = rhs.program;
    rhs.program = 0;

    return *this;
}

unsigned int Shader::handle() { return program; }

void Shader::set_int(std::string_view name, int value) {
    glUniform1i(location(name), value);
}

void Shader::set_float(std::string_view name, float value) {
    glUniform1f(location(name), value);
}
/*

void Shader::set_vec3(std::string_view name, glm::vec3 value) {
    glUniform3fv(location(name), 1, glm::value_ptr(value));
}

void Shader::set_vec4(std::string_view name, glm::vec4 value) {
    glUniform4fv(location(name), 1, glm::value_ptr(value));
}

void Shader::set_mat4(std::string_view name, glm::mat4 value) {
    glUniformMatrix4fv(location(name), 1, GL_FALSE, glm::value_ptr(value));
}*/

int Shader::location(std::string_view name) {
    Shader::use(*this); // Make sure the shader is in use
    auto loc = glGetUniformLocation(program, name.data());
    assert(loc != -1);
    return loc;
}

void Shader::use(Shader& shader) { glUseProgram(shader.program); }

} // namespace Saturn
