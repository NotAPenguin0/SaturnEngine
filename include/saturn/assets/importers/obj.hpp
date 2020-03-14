#ifndef SATURN_OBJ_IMPORTER_HPP_
#define SATURN_OBJ_IMPORTER_HPP_

#include <saturn/assets/model.hpp>
#include <saturn/utility/context.hpp>

#include <filesystem>
namespace fs = std::filesystem;

namespace saturn::assets::importers {

Model import_obj_model(Context& ctx, fs::path const& path);

}

#endif