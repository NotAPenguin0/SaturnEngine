#ifndef MVG_APPLICATION_HPP_
#define MVG_APPLICATION_HPP_

#include "Utility.hpp"

#include <GLFW\glfw3.h>
#include <string_view>

namespace Saturn {

class Application {
public:
    struct CreateInfo {
        WindowDim window_size;
        std::string_view window_caption = "";
        bool fullscreen = false;
    };

    Application(CreateInfo create_info);

	//Not copyable
    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;

	Application(Application&&);
    Application& operator=(Application&&);

	~Application();

private:
    GLFWwindow* window_handle;
    WindowDim window_dimensions;
    bool window_is_open;
};

} // namespace Saturn

#endif
