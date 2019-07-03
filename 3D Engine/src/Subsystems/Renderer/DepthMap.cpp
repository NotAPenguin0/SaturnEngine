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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    // Create the framebuffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // Attach the depth map texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           texture, 0);
    // This buffer is not used for rendering
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    // Unbind everything
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void DepthMap::bind_framebuffer(DepthMap& map) {
    glBindFramebuffer(GL_FRAMEBUFFER, map.fbo);
}

void DepthMap::unbind_framebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void DepthMap::bind_texture(DepthMap& map) {
    glBindTexture(GL_TEXTURE_2D, map.texture);
}

void DepthMap::unbind_texture() { glBindTexture(GL_TEXTURE_2D, 0); }

} // namespace Saturn
