#ifndef SATURN_TEXTURE_HPP_
#define SATURN_TEXTURE_HPP_

#include <string_view>
#include <vector>

#include "OpenGL.hpp"
#include "Utility/Utility.hpp"

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

class Texture {
public:
    struct ParameterInfo {
        TextureParameter parameter;
        TextureParameterValue value;
    };

    struct CreateInfo {
        std::string_view image_path;
        TextureTarget target;
        std::vector<ParameterInfo> parameters;
        GLenum texture_unit;
        TextureFormat format = TextureFormat::RGB;
        TextureFormat internal_format = TextureFormat::SRGBAlpha;
        bool flip_y = false; // Whether to flip the y axis on loading or not
    };

    Texture();
    Texture(CreateInfo const& create_info);
    Texture(Texture const&) = delete;
    Texture(Texture&&) = delete;

    ~Texture();

    Texture& operator=(Texture const&) = delete;
    Texture& operator=(Texture&&) = delete;

    static void bind(Texture& tex);
    static void unbind(Texture& tex);

    void assign(CreateInfo const& create_info);

    void set_parameter(ParameterInfo param);

    int unit() const;
    ImgDim dimensions() const;

    GLuint handle();

private:
    GLuint texture_handle;
    GLenum texture_unit;
    int w, h;

    TextureTarget target;
};

} // namespace Saturn

#endif
