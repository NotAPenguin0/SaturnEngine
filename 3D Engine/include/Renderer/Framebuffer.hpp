#ifndef SATURN_FRAMEBUFFER_HPP_
#define SATURN_FRAMEBUFFER_HPP_

#include "Utility/Utility.hpp"

namespace Saturn {

// TODO: Allow different settings for renderbuffer (do when we need to)
class Framebuffer {
public:
    struct CreateInfo {
        ImgDim size;
    };

    Framebuffer() = default;
    Framebuffer(CreateInfo create_info);
    Framebuffer(Framebuffer&& other) { (*this) = std::move(other); }

    Framebuffer& operator=(Framebuffer&& other) {
        if (this != &other) {
            fbo = other.fbo;
            rbo = other.rbo;
            texture = other.texture;
            size = other.size;

            other.fbo = 0;
            other.rbo = 0;
            other.texture = 0;
            other.size = {0, 0};
        }
        return *this;
    }

    ~Framebuffer();

    void assign(CreateInfo create_info);

    static void bind(Framebuffer& buf);
    static void unbind();

    ImgDim dimensions() const;

    void check_complete();

    unsigned int get_texture() { return texture; }
    void delete_texture();
    void assign_texture(unsigned int tex_handle);

    ImgDim get_size() const { return size; }
    void set_size(ImgDim sz) { size = sz; }

private:
    friend class Renderer;

    // OpenGL Framebuffer object
    unsigned int fbo = 0;
    // OpenGL Renderbuffer object
    unsigned int rbo = 0;
    // OpenGL Texture handle. This is the target we render to.
    unsigned int texture = 0;

    ImgDim size;

    void create_fbo();
    void create_rbo();
    void create_texture();
};

} // namespace Saturn

#endif
