#ifndef SATURN_PROJECT_FILE_HPP_
#define SATURN_PROJECT_FILE_HPP_

#ifdef WITH_EDITOR

#    include <filesystem>
#    include <string_view>
#    include <vector>

namespace fs = std::filesystem;

namespace Saturn::Editor {
class ProjectFile {
public:
    struct RenderStageData {
        std::string type;
        std::string stage;

        friend bool operator==(RenderStageData const& a,
                               RenderStageData const& b) {
            return a.stage == b.stage && a.type == b.type;
        }
    };

    static void load(fs::path path);
    static void save();

    static void render_stage_removed(std::string_view type,
                                     std::string_view stage);
    static void render_stage_added(std::string_view type,
                                   std::string_view stage);

    static fs::path main_scene();
    static fs::path root_path();
    static fs::path name();
    static std::string path();
    static std::vector<RenderStageData> const& get_render_stages();

private:
    static std::vector<fs::path> scene_paths;
    static fs::path self_dir;
    static std::string self_name;
    static std::vector<RenderStageData> render_stages;
};

} // namespace Saturn::Editor

#endif

#endif // ifndef SATURN_PROJECT_FILE_HPP_
