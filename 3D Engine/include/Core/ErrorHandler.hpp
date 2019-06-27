#ifndef MVG_ERROR_HANDLER_HPP_
#define MVG_ERROR_HANDLER_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Saturn {

class GLErrorHandler {
public:
	friend class Engine;

    static void initialize();

private:
    static void glfw_error_callback(int code, const char* msg);
    static void dummy_callback([[maybe_unused]] const char* name,
                               [[maybe_unused]] void* fptr,
                               [[maybe_unused]] int len_args,
                               ...);
    static void __stdcall gl_error_callback([[maybe_unused]] GLenum source,
                                  GLenum type,
                                  [[maybe_unused]] GLuint id,
                                  GLenum severity, 
								  [[maybe_unused]] GLsizei length,
                                  const GLchar* message,
                                  [[maybe_unused]] const void* userParam);
};

} // namespace Saturn

#endif
