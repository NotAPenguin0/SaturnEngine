#ifndef SATURN_UNIFORM_BUFFER_HPP_
#define SATURN_UNIFORM_BUFFER_HPP_

#include "Utility/bind_guard.hpp"

#include <glm/glm.hpp>
#include <cstddef>

namespace Saturn {

class UniformBuffer {
public:
    struct CreateInfo {
        std::size_t size_in_bytes;
        std::size_t binding_point;
        bool dynamic = false;
    };

    UniformBuffer();
    UniformBuffer(CreateInfo const& create_info);

    UniformBuffer(UniformBuffer const&) = delete;
    UniformBuffer(UniformBuffer&&) = delete;

    UniformBuffer& operator=(UniformBuffer const&) = delete;
    UniformBuffer& operator=(UniformBuffer&&) = delete;

    ~UniformBuffer();

    void assign(CreateInfo const& create_info);

    static void bind(UniformBuffer& buf);
    static void unbind();

    void set_int(int value, std::size_t byte_offset);
    void set_float(float value, std::size_t byte_offset);
    void set_vec3(glm::vec3 const& value, std::size_t byte_offset);
    void set_mat4(glm::mat4 const& value, std::size_t byte_offset);

private:
    unsigned int ubo;
};

} // namespace Saturn

#endif
