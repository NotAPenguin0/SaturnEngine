#include <saturn/codegen/display_fields_generator.hpp>

#include <saturn/codegen/utility.hpp>
#include <mustache/mustache.hpp>

#include <fstream>
#include <string>

namespace saturn::codegen {

using namespace kainjow;

static void generate_header(VisitResult const& data) {
    std::string tpl = read_file("templates/display_component_fields.hpp.mst");
    mustache::mustache mst(tpl);

    mustache::data info;
    auto& includes = info["component_includes"] = mustache::data::type::list;
    auto& decls = info["display_field_decl"] = mustache::data::type::list;

    for (auto const& component : data.components) {
        includes.push_back({"component_filename", component.filename});
        decls.push_back({"component_name", component.name});
    }

    
    std::string generated = mst.render(info);
    
    std::ofstream out("include/editor/widgets/display_component_fields.hpp");
    out << generated;
}

static void generate_source(VisitResult const& data) {
    std::string tpl = read_file("templates/display_component_fields.cpp.mst");
    mustache::mustache mst(tpl);

    mustache::data info;
    auto& impls = info["display_fields_impl"] = mustache::data::type::list;

    for (auto const& component : data.components) {
        if (component.name == "Name") { continue; }
        mustache::data comp_info;
        comp_info["component_name"] = component.name;
        auto& fields = comp_info["do_display_field"] = mustache::data::type::list;
        for (auto const& field : component.fields) {
            fields.push_back({"field_name", field.name});
        }
        impls.push_back(std::move(comp_info));
    }

    
    std::string generated = mst.render(info);
    
    std::ofstream out("src/editor/widgets/display_component_fields.cpp");
    out << generated;
}

void generate_field_display_dispatcher(VisitResult const& data) {
    generate_header(data);
    generate_source(data);
}

}