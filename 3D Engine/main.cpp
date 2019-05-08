#include "Engine.hpp"
#include "VertexArray.hpp"
#include "bind_guard.hpp"

#include "Math.hpp"

#include <cassert>
#include <iostream>

/*
 *NOTES:
 *
 * Blending, face culling and depth testing is disabled right now because 2D
 * testing without camera To renenable: Enable in Renderer::update_screen and in
 * Engine::initialize
 *
 **/

// Function try block because I'm cool
int main() try {

    Saturn::Engine::CreateInfo engine_create_info;
    engine_create_info.app_create_info.fullscreen = false;
    engine_create_info.app_create_info.window_caption = "Saturn Engine";
    engine_create_info.app_create_info.window_size = {800, 600};
    engine_create_info.enable_debug_output = true;
    Saturn::Application app = Saturn::Engine::initialize(engine_create_info);
    app.run();
} catch (Saturn::SafeTerminateException) { std::cin.ignore(32767, '\n'); }
