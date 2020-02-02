#include "Core/Engine.hpp"

#include "Utility/Utility.hpp"

#include <cassert>
#include <iostream>

#ifdef WITH_EDITOR
#include "Editor/Editor.hpp"
#endif


// Function try block because I'm cool
int main() try {
    Saturn::WindowDim window_size = {1300, 680};
    Saturn::Engine::CreateInfo engine_create_info;
    engine_create_info.app_create_info.fullscreen = false;
    engine_create_info.app_create_info.window_caption = "Saturn Engine";
    engine_create_info.app_create_info.window_size = window_size;
    engine_create_info.enable_debug_output = true;
    Saturn::Application app = Saturn::Engine::initialize(engine_create_info);

#ifdef WITH_EDITOR
    Saturn::Editor::Editor editor(app);
#endif

    app.run();

} catch (Saturn::SafeTerminateException) { std::cin.ignore(32767, '\n'); }
