#include "Renderer/Modules/DebugModule.hpp"

namespace Saturn::RenderModules {

DebugModule::DebugModule() : RenderModule(2) {}

void DebugModule::init() {}
void DebugModule::process(Scene& scene,
                          Viewport& viewport,
                          Framebuffer& target) {}

} // namespace Saturn::RenderModules
