#ifndef MVG_APPLICATION_HPP_
#define MVG_APPLICATION_HPP_

#include "Subsystems/Renderer/Renderer.hpp"
#include "Utility/Utility.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <string_view>

namespace Saturn {

/**
 * /brief Manages the application.
 *
 * This class manages the main application of your game. It also owns a window
 * with an associated context to render to.
 */
class Application {
public:
    friend class Engine;

    /**
     * /brief Creation info for the application.
     *
     * This struct contains all the data needed to initialize the application
     */
    struct CreateInfo {
        WindowDim window_size;                ///< Window dimensions
        std::string_view window_caption = ""; ///< Window caption
        bool fullscreen =
            false; ///< Whether the window should be fullscreen or not
    };

    /**
     * /brief Constructor accepting a CreateInfo struct
     */
    Application(CreateInfo create_info);

    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;

    /**
     * /brief Move constructor
     */
    Application(Application&&);

    /**
     * /brief Move assignment operator
     */
    Application& operator=(Application&&);

    /**
     * /brief Default destructor destroying the window
     */
    ~Application();

    /**
     * /brief Initializes the keybinds used in the application
     */
    void initialize_keybinds();

    /**
     * /brief Runs the application.
     *
     * Runs the application by running the main loop. Blocks the calling thread.
     */
    void run();

    /**
     * /brief Exits the application
     */
    void quit();

    /**
     * /brief Obtains a handle to the native window
     *
     * @return  A pointer to a GLFWwindow, used as a handle in the GLFW API.
     */
    GLFWwindow* window();

    /**
     * /brief Framebuffer resize callback function used by GLFW.
     *
     * @param window [unused] A handle to the window of which the framebuffer
     * was resized
     * @param w The new framebuffer width
     * @param h The new framebuffer height
     */
    void resize_callback([[maybe_unused]] GLFWwindow* window, int w, int h);

    WindowDim size() const;
	
    inline Renderer* get_renderer() { return renderer.get(); }

private:
    GLFWwindow* window_handle;   ///< Handle to the GLFW window
    WindowDim window_dimensions; ///< Size of the window
    bool window_is_open;         ///< Indicates whether the window is open

    std::unique_ptr<Renderer> renderer =
        nullptr; ///< The main renderer of the application
};

} // namespace Saturn

#endif
