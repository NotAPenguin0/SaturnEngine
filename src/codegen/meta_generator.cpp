#include <saturn/codegen/serialization_generator.hpp>

#include <mustache/mustache.hpp>
#include <saturn/codegen/utility.hpp>
#include <fstream>

namespace saturn::codegen {

using namespace kainjow;

void generate_meta_files(VisitResult const& data) {
    std::string tpl = read_file("templates/component_for_each.hpp.mst");
    mustache::mustache must(tpl);

    mustache::data info;
    auto& includes = info["component_includes"] = mustache::data::type::list;
    auto& types = info["component_types"] = mustache::data::type::list;
    for (stl::size_t i = 0; i < data.components.size(); ++i) {
        auto const& component = data.components[i];
        includes.push_back({"component_filename", component.filename});
        mustache::data type_data;
        type_data["component_name"] = component.name;
        type_data["opt_comma"] = (i == data.components.size() - 1) ? "" : ",";
        types.push_back(type_data);
    }

    std::string generated = must.render(info);
    std::ofstream out("include/saturn/meta/for_each_component.hpp");
    out << generated;
}

}