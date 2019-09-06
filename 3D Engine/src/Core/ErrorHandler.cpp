#include "Core/ErrorHandler.hpp"

#include "Editor/EditorLog.hpp"
#include "Subsystems/Logging/LogSystem.hpp"

#include <string>

namespace Saturn {

void GLErrorHandler::initialize() {}

void GLErrorHandler::glfw_error_callback(int code, const char* msg) {
    LogSystem::write(LogSystem::Severity::Warning,
                     "GLFW error with code: " + std::to_string(code));
    LogSystem::write(LogSystem::Severity::Warning, msg);
}

void GLErrorHandler::gl_error_callback([[maybe_unused]] GLenum source,
                                       GLenum type,
                                       [[maybe_unused]] GLuint id,
                                       GLenum severity,
                                       [[maybe_unused]] GLsizei length,
                                       const GLchar* message,
                                       [[maybe_unused]] const void* userParam) {

    using namespace std::literals::string_literals;

    // Set severity
    LogSystem::Severity sev = LogSystem::Severity::Info;
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        sev = LogSystem::Severity::Error;
    } else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
        sev = LogSystem::Severity::Warning;
    } else if (severity == GL_DEBUG_SEVERITY_LOW) {
        sev = LogSystem::Severity::Info;
    }
    // Set error type
    std::string errtype = "Unknown error";
    if (type == GL_DEBUG_TYPE_ERROR) {
        errtype = "Error";
    } else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) {
        errtype = "Deprecated behavior";
    } else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR) {
        errtype = "Undefined behavior";
    } else if (type == GL_DEBUG_TYPE_PORTABILITY) {
        errtype = "Non portable functionality";
    } else if (type == GL_DEBUG_TYPE_PERFORMANCE) {
        errtype = "Performance issue";
    } else if (type == GL_DEBUG_TYPE_MARKER) {
        errtype = "Command stream annotation";
    } else if (type == GL_DEBUG_TYPE_OTHER) {
        errtype = "Other error";
    }

#ifdef NO_PERFORMANCE_LOG
    if (type == GL_DEBUG_TYPE_PERFORMANCE) return;
#endif
#ifdef WITH_EDITOR
    Editor::DebugConsole::LogType log_type = Editor::DebugConsole::Info;
    switch (sev) {
        case LogSystem::Severity::Warning:
            log_type = Editor::DebugConsole::Warning;
            break;
        case LogSystem::Severity::Error:
            log_type = Editor::DebugConsole::Error;
            break;
        default: break;
    }
    log::log(fmt::format("OpenGL Debug Output"), log_type);
    log::log(fmt::format("Error Type: {}", errtype), log_type);
    log::log(fmt::format("Message: {}", message), log_type);
#else
    // Print the error message
    LogSystem::write(sev, "OpenGL Debug Output: ");
    LogSystem::write(sev, "Error type: "s + errtype);
    LogSystem::write(sev, "Message: "s + message);
#endif
}

/*

void GLErrorHandler::callback(const char* name,
                              [[maybe_unused]] void* fptr,
                              [[maybe_unused]] int len_args,
                              ...) {
    using namespace std::literals::string_literals;
    if (GLenum error = glad_glGetError(); error != GL_NO_ERROR) {
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
}*/

void GLErrorHandler::dummy_callback([[maybe_unused]] const char* name,
                                    [[maybe_unused]] void* fpts,
                                    [[maybe_unused]] int len_args,
                                    ...) {}

} // namespace Saturn
