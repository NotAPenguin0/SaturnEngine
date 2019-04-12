#ifndef MVG_ENGINE_HPP_
#define MVG_ENGINE_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Application.hpp"
#include "ErrorHandler.hpp"
#include "Input.hpp"
#include "LogSystem.hpp"
#include "Utility.hpp"
#include "Math.hpp"

#include "Renderer.hpp"

namespace Saturn {

// The main Engine class. Call Engine::initialize() to start.
class Engine {
public:
    Engine() = delete;

    // Struct storing all information the Engine needs to initialize
    struct CreateInfo {
        // Specifies the output steam to log all output to. Defaults to
        // std::cout
        std::ostream& log_target = std::cout;

        // Antialiasing level. Defaults to 4x
        std::size_t samples = 4;

        // Application create info
        Application::CreateInfo app_create_info;

		// Enable debug output. This causes a big performance drop, so be careful when using it.
		bool enable_debug_output = false;
    };

    // \brief Initializes the complete Engine.
    // \return An instance of Saturn::Application, which is used to run the
    // program
    static Application initialize(CreateInfo create_info);

private:
};

} // namespace Saturn

#endif
