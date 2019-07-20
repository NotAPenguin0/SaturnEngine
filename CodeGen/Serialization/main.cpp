#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

// libclang header
#include <clang-c/Index.h>
// mustache for generating the source code itself
#include <mustache/mustache.hpp>

namespace fs = std::filesystem;

namespace settings {

constexpr const char* component_base_file = "ComponentBase.hpp";

#define COMPONENT_MACRO "COMPONENT"

} // namespace settings

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
};

struct OutputFiles {
    fs::path header;
    fs::path source;
    fs::path scene_obj;
    fs::path components;
    fs::path component_list;
};

struct ComponentData {
    using type_name_t = std::string;

    std::string name;
    bool default_serialize;
    // Maps a member name to its type
    std::unordered_map<std::string, type_name_t> fields;
};

class TranslationUnit {
public:
    struct CompilerSettings {
        std::vector<fs::path> include_directories;
        std::vector<std::string> compiler_options;
    };

    TranslationUnit() = default;
    TranslationUnit(fs::path const& path,
                    CompilerSettings const& settings = {}) {
        index = clang_createIndex(0, 0);
        std::vector<std::string> args = get_args(settings);
        std::vector<const char*> args_c_str;
        for (auto const& arg : args) { args_c_str.push_back(arg.c_str()); }
        tu = clang_parseTranslationUnit(index, path.string().c_str(),
                                        args_c_str.data(), args.size(), nullptr,
                                        0, CXTranslationUnit_None);
        // print diagnostics
        for (unsigned int i = 0, n = clang_getNumDiagnostics(tu); i != n; ++i) {
            CXDiagnostic diag = clang_getDiagnostic(tu, i);
            CXString string = clang_formatDiagnostic(
                diag, clang_defaultDiagnosticDisplayOptions());
            fprintf(stderr, "%s\n", clang_getCString(string));
            clang_disposeString(string);
        }
    }

    ~TranslationUnit() {
        if (index && tu) {
            clang_disposeIndex(index);
            clang_disposeTranslationUnit(tu);
        }
    }

    CXIndex get_index() { return index; }
    CXTranslationUnit get_translation_unit() { return tu; }
    CXCursor get_cursor() { return clang_getTranslationUnitCursor(tu); }

private:
    std::vector<std::string> get_args(CompilerSettings const& settings) {
        std::vector<std::string> args;
        // Process include directories
        for (auto const& path : settings.include_directories) {
            std::string arg = "-I" + path.string();
            args.push_back(arg);
        }
        // Add extra compiler options
        for (auto const& arg : settings.compiler_options) {
            args.push_back(arg);
        }
        return args;
    }

    CXIndex index = nullptr;
    CXTranslationUnit tu = nullptr;
};

std::ostream& operator<<(std::ostream& out, CXString const& str) {
    out << clang_getCString(str);
    clang_disposeString(str);
    return out;
}

Directories get_directories(fs::path const& main) {
    fs::path include_directory = main / "include";
    fs::path source_directory = main / "src";
    fs::path components = include_directory / "Subsystems/ECS/Components";
    fs::path dependencies = main / "dependencies";
    fs::path glm = dependencies / "glm";
    fs::path nlohmann = dependencies / "nlohmann_json/single_include";
    fs::path glad = dependencies / "glad/include";
    fs::path glfw = dependencies / "glfw/include";
    return {main,
            include_directory,
            source_directory,
            components,
            dependencies,
            glm,
            nlohmann,
            glad,
            glfw};
}

OutputFiles get_output_files(Directories const& dirs) {
    const fs::path output_header = dirs.include / "Subsystems" /
                                   "Serialization" / "ComponentSerializers.hpp";
    const fs::path output_source = dirs.source / "Subsystems" /
                                   "Serialization" / "ComponentSerializers.cpp";
    const fs::path output_scene_obj =
        dirs.source / "Subsystems" / "Scene" / "SceneObject.cpp";
    const fs::path components =
        dirs.include / "Subsystems" / "ECS" / "Components.hpp";
    const fs::path component_list =
        dirs.include / "Subsystems" / "ECS" / "ComponentList.hpp";
    return {output_source, output_header, output_scene_obj, components,
            component_list};
}

std::vector<fs::path> get_component_files(Directories const& dirs) {
    std::vector<fs::path> result;
    try {

        for (auto const& entry : fs::directory_iterator(dirs.components)) {
            fs::path const& path = entry;
            // Exclude ComponentBase.hpp from the list, as there is no component
            // there
            if (path.filename() != settings::component_base_file) {
                result.push_back(path);
            }
        }
    } catch (fs::filesystem_error const& e) { std::cerr << e.what(); }
    return result;
}

std::string read_file_into_string(fs::path const& path) {
    std::ifstream file(path);
    if (!file.good()) {
        std::cout << "Could not open file at path: " << path << '\n';
        return "";
    }
    // https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

std::string get_component_name(std::string const& first_line) {
    static const std::size_t declaration_len =
        strlen("struct " COMPONENT_MACRO);

    std::size_t default_serialize_len = 0;
    if (first_line.find("DEFAULT_SERIALIZE") != std::string::npos) {
        default_serialize_len =
            strlen(" DEFAULT_SERIALIZE"); // Don't forget the space in front
    }

    // Find the position after the struct declaration
    const std::size_t name_start_with_spaces =
        first_line.find("struct " COMPONENT_MACRO) + declaration_len +
        default_serialize_len;
    // Find the first non-space character
    const std::size_t name_start =
        first_line.find_first_not_of(' ', name_start_with_spaces);
    const std::size_t name_end = first_line.find(' ', name_start);
    // Note that the second parameter of substr asks for a length, not a stop
    // index
    return first_line.substr(name_start, name_end - name_start);
}

bool get_component_default_serialize(std::string const& first_line) {
    if (first_line.find("DEFAULT_SERIALIZE") != std::string::npos) {
        return true;
    }

    return false;
}

TranslationUnit get_translation_unit(Directories const& dirs,
                                     fs::path const& path) {

    TranslationUnit::CompilerSettings settings;
    settings.include_directories.emplace_back(dirs.include);
    settings.include_directories.emplace_back(dirs.glm);
    settings.include_directories.emplace_back(dirs.nlohmann);
    settings.include_directories.emplace_back(dirs.glad);
    settings.include_directories.emplace_back(dirs.glfw);
    settings.compiler_options.emplace_back("-std=c++17");

    TranslationUnit tu(path, settings);

    return tu;
}

struct FieldVisitorData {
    std::string_view comp_name;
    // decltype for simplicity
    decltype(ComponentData::fields)* fields;
};

bool cursor_is_component_field(std::string_view component_name,
                               CXCursor cursor) {

    // Check if the cursor is a field declaration
    if (const CXCursorKind kind = clang_getCursorKind(cursor);
        kind == CXCursorKind::CXCursor_FieldDecl) {
        // Get the parent of this cursor to check if it is inside the component
        const CXCursor parent = clang_getCursorSemanticParent(cursor);
        const CXString parent_name = clang_getCursorSpelling(parent);

        // If the parent name is the component name, the cursor is a field of
        // the component
        if (clang_getCString(parent_name) == component_name) {
            clang_disposeString(parent_name);
            return true;
        } else {
            clang_disposeString(parent_name);
            return false;
        }
    }
    // The cursor is not even a field
    return false;
}

CXChildVisitResult
field_visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    auto& data = *static_cast<FieldVisitorData*>(client_data);

    // Check if the cursor is a field of our component
    if (cursor_is_component_field(data.comp_name, cursor)) {
        // Get necessary data
        const CXType type = clang_getCursorType(cursor);
        const CXString type_name = clang_getTypeSpelling(type);
        const CXString name = clang_getCursorSpelling(cursor);

        // Add it to our fields map
        data.fields->emplace(clang_getCString(name),
                             clang_getCString(type_name));
        // clean up the strings
        clang_disposeString(name);
        clang_disposeString(type_name);
    }

    return CXChildVisit_Recurse;
}

std::unordered_map<ComponentData::type_name_t, std::string>
get_component_fields(TranslationUnit& tu, std::string_view comp_name) {

    std::unordered_map<ComponentData::type_name_t, std::string> fields;

    // Setup visitor data
    FieldVisitorData data;
    data.comp_name = comp_name;
    data.fields = &fields;

    // Call the visitor function
    clang_visitChildren(tu.get_cursor(), field_visitor, &data);

    return fields;
}

ComponentData parse_component(Directories const& dirs,
                              fs::path const& path,
                              std::string const& first_line,
                              std::ifstream& rest_of_file) {
    ComponentData data;
    data.name = get_component_name(first_line);
    data.default_serialize = get_component_default_serialize(first_line);
    auto translation_unit = get_translation_unit(dirs, path);
    data.fields = get_component_fields(translation_unit, data.name);

    return data;
}

ComponentData process_component_file(Directories const& dirs,
                                     fs::path const& path) {
    std::ifstream file(path);

    if (!file.good()) {
        std::cerr
            << "SerializationCodeGen: Could not open component file at path: "
            << path << "\n";
        return {};
    }

    std::string line;
    while (std::getline(file, line)) {
        if (const auto pos = line.find("struct " COMPONENT_MACRO);
            pos != std::string::npos) {
            // We found a component here, now we have to parse it.
            return parse_component(dirs, path, line, file);
        }
    }

    std::cerr << "SerializationCodeGen: Error: No component found in file: "
              << path << "\n";
    return {};
}

using namespace kainjow;

void add_forward_declarations_data(
    mustache::data& data, std::vector<ComponentData> const& components) {

    data["ForwardDeclarations"] = mustache::data::type::list;
    for (auto const& component : components) {
        if (component.default_serialize) {
            data["ForwardDeclarations"].push_back(
                mustache::data("ComponentName", component.name));
        }
    }
}

void add_function_declarations_data(
    mustache::data& data, std::vector<ComponentData> const& components) {

    data["FromJsonDecl"] = mustache::data::type::list;
    data["ToJsonDecl"] = mustache::data::type::list;
    for (auto const& component : components) {
        if (component.default_serialize) {
            data["FromJsonDecl"].push_back(
                mustache::data("ComponentName", component.name));
            data["ToJsonDecl"].push_back(
                mustache::data("ComponentName", component.name));
        }
    }
}

std::string generate_header(std::vector<ComponentData> const& components) {

    static const std::string base_header = read_file_into_string("header.tpl");

    mustache::mustache header(base_header);
    mustache::data data = mustache::data::type::object;

    // Add all data
    add_forward_declarations_data(data, components);
    add_function_declarations_data(data, components);

    // Render to a string
    return header.render(data);
}

// Transforms a name from snake_case into PascalCase
std::string snake_case_to_pascal_case(std::string const& field_name) {
    std::string result;

    bool next_upper = false;
    for (char c : field_name) {
        if (c == '_') {
            next_upper = true;
        } else {
            if (next_upper) {
                result += std::toupper(c);
            } else {
                result += c;
            }

            next_upper = false;
        }
    }

    result.front() = std::toupper(result.front());

    return result;
}

void add_from_json_impl_data(mustache::data& data,
                             std::vector<ComponentData> const& components) {

    data["FromJsonImpl"] = mustache::data::type::list;
    for (auto const& component : components) {
        // Skip ParticleEmitter for now
        if (!component.default_serialize) continue;

        mustache::data impl_data = mustache::data::type::object;
        impl_data["ComponentName"] = component.name;
        impl_data["DeserializeField"] = mustache::data::type::list;

        // Add fields
        for (auto const& [field, type] : component.fields) {
            mustache::data field_data = mustache::data::type::object;

            field_data["FieldName"] = field;
            field_data["FieldType"] = type;
            field_data["FieldNameJson"] = snake_case_to_pascal_case(field);

            impl_data["DeserializeField"].push_back(field_data);
        }

        data["FromJsonImpl"].push_back(impl_data);
    }
}

void add_to_json_impl_data(mustache::data& data,
                           std::vector<ComponentData> const& components) {

    data["ToJsonImpl"] = mustache::data::type::list;
    for (auto const& component : components) {
        // Skip ParticleEmitter for now
        if (!component.default_serialize) continue;

        mustache::data impl_data = mustache::data::type::object;
        impl_data["ComponentName"] = component.name;
        impl_data["SerializeField"] = mustache::data::type::list;

        // Add fields
        for (auto const& [field, type] : component.fields) {
            mustache::data field_data = mustache::data::type::object;

            field_data["FieldName"] = field;
            field_data["FieldNameJson"] = snake_case_to_pascal_case(field);

            impl_data["SerializeField"].push_back(field_data);
        }

        data["ToJsonImpl"].push_back(impl_data);
    }
}

std::string generate_source(std::vector<ComponentData> const& components) {
    static const std::string base_source = read_file_into_string("source.tpl");

    mustache::mustache source(base_source);
    mustache::data data = mustache::data::type::object;

    // Add all data
    add_from_json_impl_data(data, components);
    add_to_json_impl_data(data, components);

    // Render to a string
    return source.render(data);
}

std::string generate_scene_obj(std::vector<ComponentData> const& components) {
    static const std::string base = read_file_into_string("scene_obj.tpl");

    mustache::mustache file(base);
    mustache::data data = mustache::data::type::object;

    // Add data
    data["DeserializeComponent"] = mustache::data::type::list;
    for (auto const& component : components) {
        mustache::data comp_data = mustache::data::type::object;
        comp_data["ComponentName"] = component.name;
        // Ugly, but I can't come up with a better way right now
        if (component.name == "Camera") {
            comp_data["AdditionalCode"] =
                "obj.get_scene()"
                "->get_app()"
                "->get_renderer()"
                "->get_viewport(component.viewport_id)"
                ".set_camera(component.id);";
        }
        data["DeserializeComponent"].push_back(comp_data);
    }

    // Render to a string
    return file.render(data);
}

std::string
generate_components_header(std::vector<ComponentData> const& components) {
    static const std::string base = read_file_into_string("components.tpl");

    mustache::mustache file(base);
    mustache::data data = mustache::data::type::object;
    data["IncludeComponentFile"] = mustache::data::type::list;

    for (auto const& component : components) {
        mustache::data comp_data = mustache::data::type::object;
        comp_data["ComponentName"] = component.name;
        data["IncludeComponentFile"].push_back(comp_data);
    }

    return file.render(data);
}

std::string
generate_component_list_header(std::vector<ComponentData> const& components) {
    // Component list will be manually generated, so no mustache templates here
    std::string header = "#ifndef MVG_COMPONENT_LIST_HPP_\n"
						 "#define MVG_COMPONENT_LIST_HPP_\n\n"
						 "#include \"Components.hpp\"\n\n";
    header += "#define COMPONENT_LIST \\ \n";
	for (std::size_t i = 0; i < components.size(); ++i) {
        auto const& name = components[i].name;

		(header += "::Saturn::Components::") += name;
		if (i != components.size() -1) { // if we're not at the last iteration 
			header += ", \\ \n";
		}
	}
    header += "\n#endif";
	return header;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: SerializationCodeGen [project_directory]\n";
        return -1;
    }
    auto directories = get_directories(argv[1]);
    auto output_files = get_output_files(directories);

    // Temporary, this is to make sure we don't overwrite our manual files with
    // wrong generated ones during testing
    output_files = {"test.hpp", "test.cpp", "scene_obj_test.cpp",
                    "components_test.hpp", "component_list_test.hpp"};

    std::vector<fs::path> component_files = get_component_files(directories);

    std::vector<ComponentData> components;
    for (auto const& file : component_files) {
        auto start = std::chrono::system_clock::now();
        components.push_back(process_component_file(directories, file));
        auto end = std::chrono::system_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();
        std::cout << "Parsed component: " << components.back().name << " in "
                  << elapsed << " ms\n";
    }

    /*for (auto const& component : components) {
        std::cout << "Component: " << component.name << "\n\n";
        std::cout << "Fields: \n";
        for (auto const& [name, type] : component.fields) {
            std::cout << type << " " << name << "\n";
        }
        std::cout << "====================================================\n";
    }*/

    std::string header = generate_header(components);
    std::string source = generate_source(components);
    std::string scene_obj = generate_scene_obj(components);
    std::string components_header = generate_components_header(components);
    std::string component_list_header =
        generate_component_list_header(components);

    std::ofstream out_header(output_files.header);
    out_header << header;
    std::ofstream out_source(output_files.source);
    out_source << source;
    std::ofstream out_scene_obj(output_files.scene_obj);
    out_scene_obj << scene_obj;
    std::ofstream out_components_header(output_files.components);
    out_components_header << components_header;
    std::ofstream out_component_list_header(output_files.component_list);
    out_component_list_header << component_list_header;

    std::cout << "Generated output files have been written to "
              << output_files.header << ", " << output_files.source << ", "
              << output_files.components << ", " << output_files.component_list
              << " and " << output_files.scene_obj
              << ".\nPress ENTER to quit\n";

    std::cin.get();
}
