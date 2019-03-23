#ifndef MVG_ENGINE_HPP_
#define MVG_ENGINE_HPP_

#include <GLFW/glfw3.h>

#include "Application.hpp"
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
