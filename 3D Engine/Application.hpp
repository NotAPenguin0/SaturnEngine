#ifndef MVG_APPLICATION_HPP_
#define MVG_APPLICATION_HPP_

#include "Renderer.hpp"
#include "Utility.hpp"

#include "glad\glad.h"
#include <GLFW\glfw3.h>
#include <memory>
#include <string_view>

namespace Saturn {

class Application {
public:
    friend class Engine;

    struct CreateInfo {
        WindowDim window_size;
        std::string_view window_caption = "";
        bool fullscreen = false;
    };

    Application(CreateInfo create_info);

    // This class is not copyable
    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;

    Application(Application&&);
    Application& operator=(Application&&);

    ~Application();

	void initialize_keybinds();

    // \brief Runs the application. Blocks the calling thread.
    void run();

    // \brief Call to quit the application.
    void quit();

    // \brief Obtains a handle to the native window
    // \return A pointer to a GLFWwindow, used as a handle in the GLFW API.
    GLFWwindow* window();

	// Framebuffer resize callback function
    void resize_callback([[maybe_unused]] GLFWwindow* window, int w, int h);

private:
    GLFWwindow* window_handle;
    WindowDim window_dimensions;
    bool window_is_open;

	std::unique_ptr<Renderer> renderer = nullptr;
};

} // namespace Saturn

#endif
