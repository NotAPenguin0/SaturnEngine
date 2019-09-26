#include "Subsystems/Renderer/Modules/RenderStage.hpp"

namespace Saturn::RenderModules {

RenderStage::RenderStage(size_t ordering) : ordering(ordering) {}

size_t RenderStage::get_ordering() const { return ordering; }

} // namespace Saturn::RenderModules
