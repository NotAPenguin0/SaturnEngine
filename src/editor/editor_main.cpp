#include <saturn/core/engine.hpp>

#include <editor/editor_system.hpp>

int main() {
    saturn::Engine engine;
    engine.get_systems().add_system<EditorSystem>();
    engine.run();
    return 0;
}