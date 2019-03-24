#include "ErrorHandler.hpp"
#include "LogSystem.hpp"

#include <string>

namespace Saturn {

void GLErrorHandler::initialize() {
    glfwSetErrorCallback(glfw_error_callback);
    // Only enable this in debug builds
#ifdef ENGINE_DEBUG_BUILD
    glad_set_post_callback(callback);
    // callback that does nothing
    glad_set_pre_callback(dummy_callback);
#else
    // Set both callbacks to do nothing
    glad_set_post_callback(dummy_callback);
    glad_set_pre_callback(dummy_callback);
#endif
}

void GLErrorHandler::glfw_error_callback(int code, const char* msg) {
    LogSystem::write(LogSystem::Severity::Warning,
                     "GLFW error with code: " + std::to_string(code));
    LogSystem::write(LogSystem::Severity::Warning, msg);
}

void GLErrorHandler::callback(const char* name,
                              [[maybe_unused]] void* fptr,
                              [[maybe_unused]] int len_args,
                              ...) {
    using namespace std::literals::string_literals;
    GLenum error = glad_glGetError();
    if (error != GL_NO_ERROR) {
        std::string errtype;
        bool fatal = false;
        switch (error) {
            case GL_INVALID_ENUM: errtype = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE: errtype = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: errtype = "GL_INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:
                errtype = "GL_OUT_OF_MEMORY";
                fatal = true;
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errtype = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        LogSystem::write(fatal ? LogSystem::Severity::FatalError
                               : LogSystem::Severity::Error,
                         "OpenGL Error " + std::to_string(error) + "(" +
                             errtype + ")");
        LogSystem::write(fatal ? LogSystem::Severity::FatalError
                               : LogSystem::Severity::Error,
                         "In function: "s + name);
    }
}

void GLErrorHandler::dummy_callback([[maybe_unused]] const char* name,
                                    [[maybe_unused]] void* fpts,
                                    [[maybe_unused]] int len_args,
                                    ...) {}

} // namespace Saturn
