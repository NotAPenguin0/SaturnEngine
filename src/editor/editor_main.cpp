#include <saturn/core/engine.hpp>

#include <editor/editor_system.hpp>
#include <editor/log_window.hpp>

#ifdef SATURN_BUILD_SAMPLES
    #include <samples/rotator_system.hpp>
#endif

int main() {
    LogWindow log_window;
    saturn::Engine engine(&log_window);
    engine.get_systems().add_system<EditorSystem>(&log_window);
#ifdef SATURN_BUILD_SAMPLES
    engine.get_systems().add_system<samples::RotatorSystem>();
#endif
    engine.run();
    return 0;
}