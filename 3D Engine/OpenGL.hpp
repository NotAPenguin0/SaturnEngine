#ifndef MVG_OPENGL_HPP_
#define MVG_OPENGL_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Saturn {

class Vao {
public:
    Vao();
    Vao(Vao const&) = delete;
    Vao(Vao&& rhs) = delete;

    Vao& operator=(Vao const&) = delete;
    Vao& operator=(Vao&& rhs) = delete;

    ~Vao();

    GLuint id = 0;

    static void bind(Vao& vao);
    static void unbind();
};

enum class BufferTarget {
    ArrayBuffer = GL_ARRAY_BUFFER,
    AtomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER,
    CopyReadBuffer = GL_COPY_READ_BUFFER,
    CopyWriteBuffer = GL_COPY_WRITE_BUFFER,
    DrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER,
    DispatchIndirectBuffer = GL_DISPATCH_INDIRECT_BUFFER,
    ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
    PixelPackBuffer = GL_PIXEL_PACK_BUFFER,
    PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
    ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER,
    TextureBuffer = GL_TEXTURE_BUFFER,
    TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,
    UniformBuffer = GL_UNIFORM_BUFFER
};

template<BufferTarget Target>
class Vbo {
public:
    Vbo() { glGenBuffers(1, &id); }
    Vbo(Vbo const&) = delete;
    Vbo(Vbo&& rhs) = delete;

    Vbo& operator=(Vbo const&) = delete;
    Vbo& operator=(Vbo&& rhs) = delete;

    ~Vbo() {
        if (id != 0) { glDeleteBuffers(1, &id); }
    }

    GLuint id = 0;
    static constexpr inline BufferTarget target = Target;

    static void bind(Vbo& vbo) {
        glBindBuffer(static_cast<GLenum>(Target), vbo.id);
    }
    static void unbind() { glBindBuffer(static_cast<GLenum>(Target), 0); }
};

using Ebo = Vbo<BufferTarget::ElementArrayBuffer>;

} // namespace Saturn

#endif
