#ifndef SATURN_PREVIEW_RENDERING_HPP_
#define SATURN_PREVIEW_RENDERING_HPP_

#include "Subsystems/AssetManager/AssetManager.hpp"
#include "Subsystems/Renderer/Mesh.hpp"

namespace Saturn::Editor {

constexpr size_t max_preview_size = 128;

unsigned int render_mesh_preview(AssetManager<Mesh>::Asset& asset);

} // namespace Saturn::Editor

#endif // ifndef SATURN_PREVIEW_RENDERING_HPP_
