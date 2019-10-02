#ifndef SATURN_PROJECT_FILE_HPP_
#define SATURN_PROJECT_FILE_HPP_

#ifdef WITH_EDITOR

#    include <filesystem>
#    include <vector>

namespace fs = std::filesystem;

namespace Saturn::Editor {
class ProjectFile {
public:
    static void load(fs::path path);

	static fs::path main_scene();
	static fs::path root_path();
	static fs::path name();

private:
    static std::vector<fs::path> scene_paths;
	static fs::path self_dir;
	static std::string self_name;
};

} // namespace Saturn::Editor

#endif

#endif // ifndef SATURN_PROJECT_FILE_HPP_
