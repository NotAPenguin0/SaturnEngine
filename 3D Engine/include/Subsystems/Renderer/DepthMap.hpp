#ifndef MVG_DEPTH_MAP_HPP_
#define MVG_DEPTH_MAP_HPP_

#include "Utility/Utility.hpp"

namespace Saturn {

class DepthMap {
public:
    struct CreateInfo {
        // The dimensions of the depth map specify how precise it will be. The
        // larger you make these, the smoother the shadows will be
        ImgDim dimensions;
    };

    DepthMap() = default;
    DepthMap(CreateInfo const& info);
    DepthMap(DepthMap const&) = delete;
    DepthMap(DepthMap&&) = delete;

    ~DepthMap();

    DepthMap& operator=(DepthMap const&) = delete;
    DepthMap& operator=(DepthMap&&) = delete;

    void assign(CreateInfo const& info);

    static void bind_framebuffer(DepthMap& map);
	static void unbind_framebuffer();

	static void bind_texture(DepthMap& map);
    static void unbind_texture();

private:
    // The depth map framebuffer
    unsigned int fbo = 0;
    // The depth map texture
    unsigned int texture = 0;
};

} // namespace Saturn

#endif
