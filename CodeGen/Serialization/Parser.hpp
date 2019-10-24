#ifndef SATURN_CODEGEN_PARSER_HPP_
#define SATURN_CODEGEN_PARSER_HPP_

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

#include <filesystem>

namespace fs = std::filesystem;

struct Directories {
    fs::path main;
    fs::path include;
    fs::path source;

    fs::path components;

    fs::path dependencies;
    fs::path glm;
    fs::path nlohmann;
    fs::path glad;
    fs::path glfw;
    fs::path audeo;
    fs::path efsw;
};

struct ComponentData {
    enum Flags : uint32_t {
        None = 0,
        DefaultSerialize = 1,
        HideInEditor = 2,
        EditorOnly = 4
    };

    struct FieldMeta {
        std::string type;
        std::string tooltip;
        bool hide_in_editor = false;
        bool do_not_serialize = false;
	};

    std::string name;
	std::string category;
    uint32_t flags;
    // Maps a member name to its type
    std::unordered_map<std::string, FieldMeta> fields;
};

ComponentData parse_component_file_new(Directories const& dirs,
                                       std::string_view fname);

#endif // ifndef SATURN_CODEGEN_PARSER_HPP_
