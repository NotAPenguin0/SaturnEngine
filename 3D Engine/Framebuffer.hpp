#ifndef MVG_FRAMEBUFFER_HPP_
#define MVG_FRAMEBUFFER_HPP_

#include "Utility.hpp"

namespace Saturn {

// TODO: Allow different settings for renderbuffer (do when we need to)
class Framebuffer : public NonCopyable {
public:
    struct CreateInfo {
        ImgDim size;
    };

	Framebuffer() = default;
    Framebuffer(CreateInfo create_info);
    Framebuffer(Framebuffer&& other);

    Framebuffer& operator=(Framebuffer&& other);

    ~Framebuffer();

    static void bind(Framebuffer const& buf);
    static void unbind();
	//Overload with unused Framebuffer argument. Equivalent to calling unbind()
	static void unbind(Framebuffer const&);

    ImgDim dimensions() const;

private:
	friend class Renderer;

    // OpenGL Framebuffer object
    unsigned int fbo;
    // OpenGL Renderbuffer object
    unsigned int rbo;
    // OpenGL Texture handle. This is the target we render to.
    unsigned int texture;

    ImgDim size;

    void create_fbo();
    void create_rbo();
    void create_texture();

    void check_complete();

	static inline unsigned int currently_bound = 0;
};

} // namespace Saturn

#endif
