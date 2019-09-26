#ifndef SATURN_FRAMEBUFFER_HPP_
#define SATURN_FRAMEBUFFER_HPP_

#include "Utility/Utility.hpp"

namespace Saturn {

// TODO: Allow different settings for renderbuffer (do when we need to)
class Framebuffer : public NonCopyable {
public:
    struct CreateInfo {
        ImgDim size;
    };

    Framebuffer() = default;
    Framebuffer(CreateInfo create_info);
    Framebuffer(Framebuffer&& other) = delete;

    Framebuffer& operator=(Framebuffer&& other) = delete;

    ~Framebuffer();

    void assign(CreateInfo create_info);

    static void bind(Framebuffer& buf);
    static void unbind();

    ImgDim dimensions() const;

    void check_complete();

    unsigned int get_texture() { return texture; }

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

    static inline unsigned int currently_bound = 0;
};

} // namespace Saturn

#endif
