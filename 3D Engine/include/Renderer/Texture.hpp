#ifndef SATURN_TEXTURE_HPP_
#define SATURN_TEXTURE_HPP_

#include <string_view>
#include <vector>

#include "OpenGL.hpp"
#include "Utility/Utility.hpp"

namespace Saturn {


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

    void swap(Texture& other) {
        std::swap(texture_handle, other.texture_handle);
        std::swap(texture_unit, other.texture_unit);
        std::swap(w, other.w);
        std::swap(h, other.h);
        std::swap(target, other.target);
    }

private:
    GLuint texture_handle;
    GLenum texture_unit;
    int w, h;

    TextureTarget target;
};

} // namespace Saturn

#endif
