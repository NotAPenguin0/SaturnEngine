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

// mustache for generating the source code itself
#include <mustache/mustache.hpp>

#include "Parser.hpp"

namespace fs = std::filesystem;

template<typename string_type>
std::vector<string_type> split(string_type const& s,
                               typename string_type::value_type delim) {
    std::vector<string_type> elems;
    std::basic_stringstream<typename string_type::value_type> ss(s);
    string_type item;
    while (std::getline(ss, item, delim)) { elems.push_back(item); }
    return elems;
}

template<typename string_type>
string_type join(std::vector<string_type> const& v,
                 typename string_type::value_type sep = ' ') {
    string_type result;
    for (auto elem : v) {
        result += elem;
        result += sep;
    }
    result.pop_back();
    return result;
}

std::string bool_str(bool b) { return b ? "true"s : "false"s; }

namespace settings {

constexpr const char* component_base_file = "ComponentBase.hpp";

} // namespace settings

struct OutputFiles {
    fs::path header;
    fs::path source;
    fs::path scene_obj;
    fs::path components;
    fs::path component_list;
    fs::path component_meta_src;
    fs::path component_meta_header;
};


Directories get_directories(fs::path const& main) {
    fs::path include_directory = main / "include";
    fs::path source_directory = main / "src";
    fs::path components = include_directory / "ECS/Components";
    fs::path dependencies = main / "dependencies";
    fs::path glm = dependencies / "glm";
    fs::path nlohmann = dependencies / "nlohmann_json/single_include";
    fs::path glad = dependencies / "glad/include";
    fs::path glfw = dependencies / "glfw/include";
    fs::path audeo = dependencies / "audeo/include";
    fs::path efsw = dependencies / "efsw/include";
    return {main,
            include_directory,
            source_directory,
            components,
            dependencies,
            glm,
            nlohmann,
            glad,
            glfw,
            audeo,
            efsw};
}

OutputFiles get_output_files(Directories const& dirs) {
    const fs::path output_header =
        dirs.include / "Serialization" / "ComponentSerializers.hpp";
    const fs::path output_source =
        dirs.source / "Serialization" / "ComponentSerializers.cpp";
    const fs::path output_scene_obj = dirs.source / "Scene" / "SceneObject.cpp";
    const fs::path components = dirs.include / "ECS" / "Components.hpp";
    const fs::path component_list = dirs.include / "ECS" / "ComponentList.hpp";
    const fs::path component_meta_src =
        dirs.source / "Serialization" / "ComponentMetaInfo.cpp";
    const fs::path component_meta_header =
        dirs.include / "Serialization" / "ComponentMetaInfo.hpp";
    return {output_header,        output_source,  output_scene_obj,
            components,           component_list, component_meta_src,
            component_meta_header};
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
        std::cerr << "Could not open file at path: " << path << '\n';
        return "";
    }
    // https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

using namespace kainjow;

void add_forward_declarations_data(
    mustache::data& data, std::vector<ComponentData> const& components) {

    data["ForwardDeclarations"] = mustache::data::type::list;
    for (auto const& component : components) {
        if (component.flags & ComponentData::DefaultSerialize) {
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
        if (component.flags & ComponentData::DefaultSerialize) {
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
        if (!(component.flags & ComponentData::DefaultSerialize)) continue;

        mustache::data impl_data = mustache::data::type::object;
        impl_data["ComponentName"] = component.name;
        impl_data["DeserializeField"] = mustache::data::type::list;

        // Add fields
        for (auto const& [field, meta] : component.fields) {
            mustache::data field_data = mustache::data::type::object;
            if (meta.type == "audeo::Sound") continue;
            field_data["FieldName"] = field;
            field_data["FieldType"] = meta.type;
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
        if (!(component.flags & ComponentData::DefaultSerialize)) continue;

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
            comp_data["AdditionalCode"] = "obj.get_scene()"
                                          "->get_app()"
                                          "->get_renderer()"
                                          "->get_viewport(c.viewport_id)"
                                          ".set_camera(c.id);";
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
    header += "#define COMPONENT_LIST \\\n";
    for (std::size_t i = 0; i < components.size(); ++i) {
        auto const& name = components[i].name;

        (header += "::Saturn::Components::") += name;
        if (i != components.size() - 1) { // if we're not at the last iteration
            header += ", \\\n";
        }
    }
    header += "\n#endif";
    return header;
}

std::string
generate_components_meta_info(std::vector<ComponentData> const& components) {
    static const std::string base = read_file_into_string("meta_src.tpl");
    mustache::mustache source(base);

    mustache::data data = mustache::data::type::object;
    auto& meta_data = data["ComponentMeta"] = mustache::data::type::list;
    for (auto const& component : components) {
        mustache::data comp_data = mustache::data::type::object;
        comp_data["ComponentName"] = component.name + " ";
        bool default_serialize =
            component.flags & ComponentData::DefaultSerialize;
        bool hide_in_editor = component.flags & ComponentData::HideInEditor;
        bool editor_only = component.flags & ComponentData::EditorOnly;
        comp_data["DefaultSerialize"] = bool_str(default_serialize);
        comp_data["HideInEditor"] = bool_str(hide_in_editor);
        comp_data["EditorOnly"] = bool_str(editor_only);
        comp_data["ComponentFieldMeta"] = mustache::data::type::list;
        auto& fields_data_list = comp_data["ComponentFieldMeta"];
        for (auto const& [field_name, field_meta] : component.fields) {
            if (field_meta.type == "audeo::Sound") continue;
            mustache::data field_data = mustache::data::type::object;
            field_data["FieldName"] = field_name;
            field_data["FieldType"] = field_meta.type;
            field_data["FieldTooltip"] = field_meta.tooltip;
            fields_data_list.push_back(field_data);
        }

        meta_data.push_back(comp_data);
    }

    return source.render(data);
}

std::string
generate_meta_info_header(std::vector<ComponentData> const& components) {
    static const std::string base = read_file_into_string("meta_header.tpl");
    mustache::mustache source(base);

    mustache::data data = mustache::data::type::object;
    data["ComponentFieldOverload"] = mustache::data::type::list;
    auto& list = data["ComponentFieldOverload"];

    for (auto const& component : components) {
        mustache::data c_data = mustache::data::type::object;
        c_data["ComponentName"] = component.name;
        // #TODO: Skipping particle emitter for now, needs special treatment
        if (component.name == "ParticleEmitter") continue;
        c_data["Field"] = mustache::data::type::list;
        auto& field_list = c_data["Field"];
        for (auto const& [field_name, field_meta] : component.fields) {
            if (field_meta.type == "audeo::Sound") continue;
            mustache::data f_data = mustache::data::type::object;
            f_data["FieldName"] = field_name;
            field_list.push_back(f_data);
        }
        list.push_back(c_data);
    }

    return source.render(data);
}

void write_output_file(fs::path const& out, std::string const& content) {
    // Check if the output file is the same as our content that we are going to
    // write. This helps avoid a recompilation
    const auto old = read_file_into_string(out);
    if (old == content) {
        // Skip writing to this file
        return;
    }
    std::ofstream file(out);
    file.clear();
    file << content;

    std::cout << "Finished writing data to file " << out << "\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: SerializationCodeGen [project_directory]\n";
        return -1;
    }
    auto directories = get_directories(argv[1]);
    auto output_files = get_output_files(directories);

    std::vector<fs::path> component_files = get_component_files(directories);

    std::vector<ComponentData> components;
    for (auto const& file : component_files) {
        auto start = std::chrono::system_clock::now();
        //        components.push_back(process_component_file(directories,
        //        file));
        //
        auto new_parsed =
            parse_component_file_new(directories, file.generic_string());
        components.push_back(new_parsed);

        auto end = std::chrono::system_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();
        std::cout << "Parsed component: " << components.back().name << " in "
                  << elapsed << " ms\n";
        std::cout.flush();
    }

    std::string header = generate_header(components);
    std::string source = generate_source(components);
    std::string scene_obj = generate_scene_obj(components);
    std::string components_header = generate_components_header(components);
    std::string component_list_header =
        generate_component_list_header(components);
    std::string meta_source = generate_components_meta_info(components);
    std::string meta_header = generate_meta_info_header(components);

    // Write output with small cache check
    write_output_file(output_files.header, header);
    write_output_file(output_files.source, source);
    write_output_file(output_files.scene_obj, scene_obj);
    write_output_file(output_files.components, components_header);
    write_output_file(output_files.component_list, component_list_header);
    write_output_file(output_files.component_meta_src, meta_source);
    write_output_file(output_files.component_meta_header, meta_header);
}
