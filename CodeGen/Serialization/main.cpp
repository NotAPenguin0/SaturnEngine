#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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
};

struct OutputFiles {
    fs::path source;
    fs::path header;
};

struct ComponentData {
    using type_name_t = std::string;

    std::string name;
    // bool default_serialize
    std::unordered_map<type_name_t, std::string> members;
};

Directories get_directories(fs::path main) {
    fs::path include_directory = main / "include";
    fs::path source_directory = main / "src";
    fs::path components = include_directory / "Subsystems/ECS/Components";
    return {main, include_directory, source_directory, components};
}

OutputFiles get_output_files(Directories const& dirs) {
    fs::path output_header = dirs.include / "Subsystems" / "Serialization" /
                             "ComponentSerializers.hpp";
    fs::path output_source = dirs.source / "Subsystems" / "Serialization" /
                             "ComponentSerializers.cpp";
    return {output_source, output_header};
}

std::vector<fs::path> get_component_files(Directories const& dirs) {
    std::vector<fs::path> result;
    for (auto const& entry : fs::directory_iterator(dirs.components)) {
        fs::path const& path = entry;
        // Exclude ComponentBase.hpp from the list, as there is no component
        // there
        if (path.filename() != settings::component_base_file) {
            result.push_back(path);
        }
    }
    return result;
}

ComponentData process_component_file(fs::path path) {
    std::ifstream file(path);

    if (!file.good()) {
        std::cerr
            << "SerializationCodeGen: Could not open component file at path: "
            << path << "\n";
        return {};
    }

    std::string line;
    while (std::getline(file, line)) {
        if (auto pos = line.find("struct" COMPONENT_MACRO); pos != std::string::npos) {
			// We found a component here, now we have to parse it. 
		}
    }
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
        components.push_back(process_component_file(file));
    }
}
