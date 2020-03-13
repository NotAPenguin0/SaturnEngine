#include <saturn/core/engine.hpp>

#include <editor/editor_system.hpp>
#include <editor/log_window.hpp>

int main() {
    LogWindow log_window;
    saturn::Engine engine(&log_window);
    engine.get_systems().add_system<EditorSystem>(&log_window);
    engine.run();
    return 0;
}