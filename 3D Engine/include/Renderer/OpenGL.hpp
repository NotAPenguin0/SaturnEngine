#ifndef SATURN_OPENGL_HPP_
#define SATURN_OPENGL_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>


namespace Saturn {


enum class TextureTarget {
    Texture1D = GL_TEXTURE_1D,
    Texture1DArray = GL_TEXTURE_1D_ARRAY,
    Texture2D = GL_TEXTURE_2D,
    Texture2DArray = GL_TEXTURE_2D_ARRAY,
    Texture2DMultiSample = GL_TEXTURE_2D_MULTISAMPLE,
    Texture2DMultiSampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
    Texture3D = GL_TEXTURE_3D,
    CubeMap = GL_TEXTURE_CUBE_MAP,
    CubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
    TextureRectangle = GL_TEXTURE_RECTANGLE
};

enum class TextureParameter {
    DepthStencilMode = GL_DEPTH_STENCIL_TEXTURE_MODE,
    BaseLevel = GL_TEXTURE_BASE_LEVEL,
    CompareFunc = GL_TEXTURE_COMPARE_FUNC,
    CompareMode = GL_TEXTURE_COMPARE_MODE,
    LodBias = GL_TEXTURE_LOD_BIAS,
    MinFilter = GL_TEXTURE_MIN_FILTER,
    MagFilter = GL_TEXTURE_MAG_FILTER,
    MinLod = GL_TEXTURE_MIN_LOD,
    MaxLod = GL_TEXTURE_MAX_LOD,
    MaxLevel = GL_TEXTURE_MAX_LEVEL,
    SwizzleR = GL_TEXTURE_SWIZZLE_R,
    SwizzleG = GL_TEXTURE_SWIZZLE_G,
    SwizzleB = GL_TEXTURE_SWIZZLE_B,
    SwizzleA = GL_TEXTURE_SWIZZLE_A,
    WrapS = GL_TEXTURE_WRAP_S,
    WrapT = GL_TEXTURE_WRAP_T,
    WrapR = GL_TEXTURE_WRAP_R
};

enum class TextureParameterValue {
    DepthComponent = GL_DEPTH_COMPONENT,
    // Mipmap filters
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
    LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    // Wrapping options
    ClampToEdge = GL_CLAMP_TO_EDGE,
    ClampToBorder = GL_CLAMP_TO_BORDER,
    MirroredRepeat = GL_MIRRORED_REPEAT,
    Repeat = GL_REPEAT,
};

enum class TextureFormat {
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    SRGB = GL_SRGB,
    SRGBAlpha = GL_SRGB_ALPHA
};

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
	// UniformBuffer -> use specialized class
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
