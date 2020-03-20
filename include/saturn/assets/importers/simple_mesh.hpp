#ifndef SATURN_SIMPLE_MESH_IMPORTER_HPP_
#define SATURN_SIMPLE_MESH_IMPORTER_HPP_

#include <saturn/utility/context.hpp>

#include <phobos/forward.hpp>
#include <filesystem>

#include <optional>

namespace fs = std::filesystem;

namespace saturn {

namespace assets::importers {

std::optional<ph::Mesh> import_simple_mesh(Context& ctx, fs::path const& path);

}

}

#endif