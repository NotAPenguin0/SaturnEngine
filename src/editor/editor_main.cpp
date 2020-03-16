#include <saturn/core/engine.hpp>

#include <editor/systems/editor_system.hpp>
#include <editor/systems/camera.hpp>
#include <editor/widgets/log_window.hpp>

#ifdef SATURN_BUILD_SAMPLES
    #include <samples/rotator_system.hpp>
#endif

int main() {
    editor::LogWindow log_window;
    saturn::Engine engine(&log_window);
    engine.get_systems().add_system<editor::EditorSystem>(&log_window);
    engine.get_systems().add_system<editor::CameraSystem>();
#ifdef SATURN_BUILD_SAMPLES
    engine.get_systems().add_system<samples::RotatorSystem>();
#endif
    engine.run();
    return 0;
}