#ifndef MVG_ERROR_HANDLER_HPP_
#define MVG_ERROR_HANDLER_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Saturn {

class GLErrorHandler {
public:
    static void initialize();

private:
    static void callback(const char* name, void* fptr, int len_args, ...);
    static void glfw_error_callback(int code, const char* msg);
    static void dummy_callback(const char* name, void* fpts, int len_args, ...);
};

} // namespace Saturn

#endif
