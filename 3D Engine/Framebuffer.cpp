#include "Framebuffer.hpp"

#include "LogSystem.hpp"
#include "OpenGL.hpp"

namespace Saturn {


Framebuffer::Framebuffer(CreateInfo create_info) : size(create_info.size) {

    // Create the framebuffer and bind it
    create_fbo();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    create_texture();
    // Attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture, 0);
    create_rbo();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ImgDim Framebuffer::dimensions() const { return size; }

void Framebuffer::create_fbo() { glGenFramebuffers(1, &fbo); }

void Framebuffer::create_rbo() {
    // Create the renderbuffer and bind it
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // Set storage mode
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, rbo);
    // Unbind the renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::create_texture() {
    // Create the texture to render to and bind it
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Make the texture empty
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // We're done with the texture, so unbind it
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::check_complete() {
    auto framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (framebuffer_status == GL_FRAMEBUFFER_COMPLETE) {
        return;
    } else {
        // Yikes
        LogSystem::write(LogSystem::Severity::Error,
                         "Failed to create framebuffer. Reason: ");
        if (framebuffer_status == GL_FRAMEBUFFER_UNDEFINED) {
            LogSystem::write(LogSystem::Severity::Error,
                             "GL_FRAMEBUFFER_UNDEFINED");
        } else if (framebuffer_status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
            LogSystem::write(LogSystem::Severity::Error,
                             "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
        } else if (framebuffer_status ==
                   GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
            LogSystem::write(LogSystem::Severity::Error,
                             "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
        } else if (framebuffer_status ==
                   GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER) {
            LogSystem::write(LogSystem::Severity::Error,
                             "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
        } else if (framebuffer_status ==
                   GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER) {
            LogSystem::write(LogSystem::Severity::Error,
                             "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
        } else if (framebuffer_status == GL_FRAMEBUFFER_UNSUPPORTED) {
            LogSystem::write(LogSystem::Severity::Error,
                             "GL_FRAMEBUFFER_UNSUPPORTED");
        } else if (framebuffer_status ==
                   GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE) {
            LogSystem::write(LogSystem::Severity::Error,
                             "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
        } else if (framebuffer_status ==
                   GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS) {
            LogSystem::write(LogSystem::Severity::Error,
                             "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
        }
    }
}

} // namespace Saturn
