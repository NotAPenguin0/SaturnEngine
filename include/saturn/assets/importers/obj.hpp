#ifndef SATURN_OBJ_IMPORTER_HPP_
#define SATURN_OBJ_IMPORTER_HPP_

#include <saturn/assets/model.hpp>
#include <saturn/utility/context.hpp>

#include <saturn/ecs/entity.hpp>

#include <filesystem>
namespace fs = std::filesystem;

namespace saturn::assets::importers {

// Imports a model into an existing entity tree
Model import_obj_model(ecs::entity_t root, Context& ctx, fs::path const& path);
Model import_obj_model(Context& ctx, fs::path const& path);

}

#endif