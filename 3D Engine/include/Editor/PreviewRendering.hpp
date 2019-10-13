#ifndef SATURN_PREVIEW_RENDERING_HPP_
#define SATURN_PREVIEW_RENDERING_HPP_

#include "AssetManager/AssetManager.hpp"
#include "Renderer/Mesh.hpp"

namespace Saturn::Editor {

namespace previews {

constexpr size_t max_preview_size = 256;

unsigned int render_mesh_preview(AssetManager<Mesh>::Asset& asset);

void clear_cache();

} // namespace previews

} // namespace Saturn::Editor

#endif // ifndef SATURN_PREVIEW_RENDERING_HPP_
