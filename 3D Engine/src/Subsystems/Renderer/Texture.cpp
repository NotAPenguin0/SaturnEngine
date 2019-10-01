#include "Subsystems/Renderer/Texture.hpp"

#include "Subsystems/Logging/LogSystem.hpp"
#include "Subsystems/Renderer/stb_image.h"

#include "Editor/EditorLog.hpp"

namespace Saturn {

Texture::Texture() :
    texture_handle(0), texture_unit(static_cast<GLenum>(-1)), w(0), h(0) {}

Texture::Texture(CreateInfo const& create_info) { assign(create_info); }

void Texture::assign(CreateInfo const& create_info) {
    //    if (texture_handle != 0) { glDeleteTextures(1, &texture_handle); }

    texture_handle = 0;
    texture_unit = create_info.texture_unit;
    target = create_info.target;

    stbi_set_flip_vertically_on_load(create_info.flip_y);
    std::int32_t desired_channel_count = 4;
    std::int32_t channels;
    unsigned char* image_data = stbi_load(create_info.image_path.data(), &w, &h,
                                          &channels, desired_channel_count);
    if (image_data == nullptr) {
        log::error(fmt::format("Failed to open file at path: {}",
                               create_info.image_path));
        return;
    }

    glGenTextures(1, &texture_handle);
    glBindTexture(static_cast<GLenum>(create_info.target), texture_handle);
    for (ParameterInfo param : create_info.parameters) {
        glTexParameteri(static_cast<GLenum>(create_info.target),
                        static_cast<GLenum>(param.parameter),
                        static_cast<int>(param.value));
    }

    if (target != TextureTarget::Texture2D) {
        throw std::runtime_error("Currently only 2D textures are supported");
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0,
                     static_cast<GLenum>(create_info.internal_format), w, h, 0,
                     static_cast<GLenum>(create_info.format), GL_UNSIGNED_BYTE,
                     image_data);
    }
    // Don't forget to change if we support more textures later
    glGenerateMipmap(GL_TEXTURE_2D);

    // memory is now on the GPU, we don't need it here anymore
    stbi_image_free(image_data);
}

Texture::~Texture() {
    if (texture_handle != 0) { glDeleteTextures(1, &texture_handle); }
}

void Texture::bind(Texture& tex) {
    glActiveTexture(tex.unit());
    glBindTexture(static_cast<GLenum>(tex.target), tex.texture_handle);
}

void Texture::unbind(Texture& tex /* = TextureTarget::Texture2D*/) {
    glActiveTexture(tex.unit());
    glBindTexture(static_cast<GLenum>(tex.target), 0);
}

void Texture::set_parameter(ParameterInfo param) {
    bind(*this);
    glTexParameteri(static_cast<GLenum>(target),
                    static_cast<GLenum>(param.parameter),
                    static_cast<int>(param.value));
    unbind(*this);
}

int Texture::unit() const { return static_cast<int>(texture_unit); }

ImgDim Texture::dimensions() const {
    ImgDim dim;
    dim.x = (unsigned)w;
    dim.y = (unsigned)h;
    return dim;
}

GLuint Texture::handle() { return texture_handle; }

} // namespace Saturn
