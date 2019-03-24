#include "Engine.hpp"

#include <iostream>

// Function try block because I'm cool
int main() try {
    Saturn::Engine::CreateInfo engine_create_info;
    engine_create_info.app_create_info.fullscreen = false;
    engine_create_info.app_create_info.window_caption = "Saturn Engine";
    engine_create_info.app_create_info.window_size = {800, 600};
    Saturn::Application app = Saturn::Engine::initialize(engine_create_info);
    app.run();
} catch (Saturn::SafeTerminateException) { std::cin.ignore(32767, '\n'); }
