#include "Editor/ProjectFile.hpp"

#ifdef WITH_EDITOR

#    include <fstream>

#    include "Editor/EditorLog.hpp"

#    include "Renderer/CubeMap.hpp"
#    include "Renderer/Font.hpp"
#    include "Renderer/Material.hpp"
#    include "Renderer/Mesh.hpp"
#    include "Renderer/Shader.hpp"
#    include "Renderer/Texture.hpp"
#    include <audeo/SoundSource.hpp>

#    include "AssetManager/AssetManager.hpp"

namespace Saturn::Editor {

std::vector<fs::path> ProjectFile::scene_paths;
fs::path ProjectFile::self_dir;
std::string ProjectFile::self_name;
std::vector<ProjectFile::RenderStageData> ProjectFile::render_stages;

void ProjectFile::load(fs::path path) {
    // Reset
    self_name = "";
    self_dir = "";
    scene_paths.clear();
    render_stages.clear();

    std::ifstream file(path);
    if (!file.good()) {
        log::error("Failed to open project file at path {}", path.string());
        return;
    }

    AssetManager<Shader>::resource_list().clear();
    AssetManager<Texture>::resource_list().clear();
    AssetManager<Mesh>::resource_list().clear();
    AssetManager<audeo::SoundSource>::resource_list().clear();
    AssetManager<Font>::resource_list().clear();
    AssetManager<CubeMap>::resource_list().clear();
    AssetManager<Material>::resource_list().clear();

    AssetManager<Shader>::get_id_map().clear();
    AssetManager<Texture>::get_id_map().clear();
    AssetManager<Mesh>::get_id_map().clear();
    AssetManager<audeo::SoundSource>::get_id_map().clear();
    AssetManager<Font>::get_id_map().clear();
    AssetManager<CubeMap>::get_id_map().clear();
    AssetManager<Material>::get_id_map().clear();

    self_name = path.stem().string();
    self_dir = fs::absolute(path.remove_filename());

    size_t scene_count;
    file >> scene_count;
    std::string scene_path;
    file.ignore(32767, '\n');
    while (scene_count-- > 0 && std::getline(file, scene_path)) {
        scene_paths.emplace_back(fs::absolute(self_dir / scene_path));
    }
    size_t stage_count;
    file >> stage_count;
    file.ignore(32767, '\n');
    std::string type, stage;
    while (stage_count-- > 0 && file >> type >> stage) {
        render_stages.push_back(
            RenderStageData{std::move(type), std::move(stage)});
    }

    // Submit asset import lists
    std::string id, asset_type, asset_path;
    while (file >> id && id == "asset" && file >> asset_type &&
           std::getline(file, asset_path)) {
        asset_path = asset_path.substr(1);
        if (asset_type == "shader") {
            AssetManager<Shader>::queue_import(asset_path);
        }
        if (asset_type == "texture") {
            AssetManager<Texture>::queue_import(asset_path);
        }
        if (asset_type == "mesh") {
            AssetManager<Mesh>::queue_import(asset_path);
        }
        if (asset_type == "sound") {
            AssetManager<audeo::SoundSource>::queue_import(asset_path);
        }
        if (asset_type == "font") {
            AssetManager<Font>::queue_import(asset_path);
        }
        if (asset_type == "cubemap") {
            AssetManager<CubeMap>::queue_import(asset_path);
        }
		if (asset_type == "material") {
			AssetManager<Material>::queue_import(asset_path);
		}
    }
}

namespace {

struct ExportedAsset {
    fs::path path;
    std::string type;
};

template<typename T>
void export_assets(std::vector<ExportedAsset>& assets, std::string_view type) {

    for (auto const& [id, asset] : AssetManager<T>::resource_list()) {
        if (asset.imported) {
            assets.push_back(ExportedAsset{
                fs::relative(asset.path, ProjectFile::root_path()),
                std::string(type)});
        }
    }
}

} // namespace

void ProjectFile::save() {
    std::ofstream file(path());

    file << scene_paths.size() << "\n";
    for (auto const& path : scene_paths) {
        file << fs::relative(path, root_path()).string() << "\n";
    }
    file << render_stages.size() << "\n";
    for (auto const& stage : render_stages) {
        file << stage.type << " " << stage.stage << "\n";
    }

    std::vector<ExportedAsset> assets;
    export_assets<Shader>(assets, "shader");
    export_assets<Texture>(assets, "texture");
    export_assets<Mesh>(assets, "mesh");
    export_assets<audeo::SoundSource>(assets, "sound");
    export_assets<Font>(assets, "font");
    export_assets<CubeMap>(assets, "cubemap");
	export_assets<Material>(assets, "material");

    // Write exported assets to file
    for (auto& exported_asset : assets) {
        file << "asset " << exported_asset.type << " "
             << exported_asset.path.generic_string() << "\n";
    }
}

void ProjectFile::create_and_load(fs::path folder) {
    auto project_name = folder.stem().generic_string() + ".seproj";
    std::ofstream file(folder / project_name);

    fs::create_directories(folder / "scenes");
    fs::create_directories(folder / "assets");
    fs::create_directories(folder / "scenes" / "main_scene");

    file << 1 << "\nscenes/main_scene\n";
    file << "7\n"
            "PreRenderStage DepthMapPass\nRenderModule DebugModule\n"
            "RenderModule MeshRenderModule\nRenderModule "
            "ParticleModule\nRenderModule SkyboxPass\n"
            "RenderModule TransferModule\nPostRenderStage "
            "BlitPass\nPostRenderStage UIPass\n"
            "RenderModule EditorModule\n";
    file.close();
    load(folder / project_name);
    log::log("Successfully created project {}", project_name);
}

void ProjectFile::render_stage_removed(std::string_view type,
                                       std::string_view stage) {
    for (auto it = render_stages.begin(); it != render_stages.end(); ++it) {
        if (it->stage == stage && it->type == type) {
            render_stages.erase(it);
            break;
        }
    }
}

void ProjectFile::render_stage_added(std::string_view type,
                                     std::string_view stage) {
    if (std::find(render_stages.begin(), render_stages.end(),
                  RenderStageData{std::string(type), std::string(stage)}) !=
        render_stages.end()) {
        return;
    }
    render_stages.push_back(
        RenderStageData{std::string(type), std::string(stage)});
}

fs::path ProjectFile::main_scene() {
    if (scene_paths.empty()) {
        log::error("Project has no scenes");
        return "";
    }

    return scene_paths.front();
}

fs::path ProjectFile::root_path() { return self_dir; }

fs::path ProjectFile::name() { return self_name; }

std::string ProjectFile::path() {
    return (root_path() / name()).string() + ".seproj";
}

std::vector<ProjectFile::RenderStageData> const&
ProjectFile::get_render_stages() {
    return render_stages;
}

std::vector<fs::path>& ProjectFile::get_scene_paths() { return scene_paths; }

} // namespace Saturn::Editor

#endif
