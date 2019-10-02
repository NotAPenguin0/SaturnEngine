#include "Editor/ProjectFile.hpp"

#ifdef WITH_EDITOR

#    include <fstream>

#    include "Editor/EditorLog.hpp"

namespace Saturn::Editor {

std::vector<fs::path> ProjectFile::scene_paths;
fs::path ProjectFile::self_dir;
std::string ProjectFile::self_name;

void ProjectFile::load(fs::path path) {

    self_name = "";
    self_dir = "";
    scene_paths.clear();

    std::ifstream file(path);
    if (!file.good()) {
        log::error("Failed to open project file at path {}", path.string());
        return;
    }

    self_name = path.stem().string();
    self_dir = fs::absolute(path.remove_filename());

    size_t scene_count;
    file >> scene_count;
    std::string scene_path;
    file.ignore(32767, '\n');
    while (scene_count --> 0 && std::getline(file, scene_path)) {
        scene_paths.emplace_back(fs::absolute(self_dir / scene_path));
    }
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

} // namespace Saturn::Editor

#endif
