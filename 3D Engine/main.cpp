#include "Engine.hpp"

#include <iostream>

constexpr int cexpr() {
    Saturn::vec4f v(1.0f);
    Saturn::vec4f f(2.0f);
    return static_cast<int>(Saturn::Math::dot(v, f));
}

// Function try block because I'm cool
int main() try {
    int arr[cexpr()];
    Saturn::Engine::CreateInfo engine_create_info;
    engine_create_info.app_create_info.fullscreen = false;
    engine_create_info.app_create_info.window_caption = "Saturn Engine";
    engine_create_info.app_create_info.window_size = {800, 600};
    engine_create_info.enable_debug_output = true;
    Saturn::Application app = Saturn::Engine::initialize(engine_create_info);
    app.run();
} catch (Saturn::SafeTerminateException) { std::cin.ignore(32767, '\n'); }
