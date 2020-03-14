#ifndef SATURN_STB_TEXTURE_IMPORT_HPP_
#define SATURN_STB_TEXTURE_IMPORT_HPP_

#include <phobos/forward.hpp>
#include <saturn/utility/context.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace saturn::assets::importers {

ph::Texture import_with_stb(Context& ctx, fs::path const& path);

}

#endif