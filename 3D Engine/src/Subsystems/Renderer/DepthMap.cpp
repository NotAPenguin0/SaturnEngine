#include "Subsystems/Renderer/DepthMap.hpp"

#include "Subsystems/Renderer/OpenGL.hpp"

namespace Saturn {

DepthMap::DepthMap(CreateInfo const& info) { assign(info); }

DepthMap::~DepthMap() {
    if (fbo != 0) { glDeleteFramebuffers(1, &fbo); }
    if (texture != 0) { glDeleteTextures(1, &texture); }
}

void DepthMap::assign(CreateInfo const& info) {
    // Create the texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
	// Internal format is GL_DEPTH_COMPONENT because we are making a depth map
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, info.dimensions.x,
                 info.dimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
}

} // namespace Saturn
