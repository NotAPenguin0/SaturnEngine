#ifndef MVG_ENGINE_HPP_
#define MVG_ENGINE_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Input.hpp"
#include "Application.hpp"
#include "ErrorHandler.hpp"
#include "LogSystem.hpp"
#include "Utility.hpp"

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
    };

    // \brief Initializes the complete Engine.
    // \return An instance of Saturn::Application, which is used to run the
    // program
    static Application initialize(CreateInfo create_info);

private:
};

} // namespace Saturn

#endif
