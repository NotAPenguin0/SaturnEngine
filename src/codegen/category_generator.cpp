#include <saturn/codegen/category_generator.hpp>

#include <saturn/codegen/utility.hpp>
#include <mustache/mustache.hpp>

#include <fstream>
#include <string>

namespace saturn::codegen {

void generate_type_category_file(VisitResult const& data) {
    using namespace kainjow;
    std::string tmpl = read_file("templates/type_category.mst");
    mustache::mustache mst(tmpl);

    mustache::data mst_data;
    auto& includes = mst_data["component_includes"] = mustache::data::type::list;
    auto& impls = mst_data["component_category_impl"] = mustache::data::type::list;
    for (auto const& component : data.components) {
        includes.push_back({"component_filename", component.filename});
        impls.push_back({"component_unscoped_name", component.unscoped_name});
    }

    std::string generated = mst.render(mst_data);

    std::ofstream out("include/saturn/meta/type_category.hpp");
    out << generated;
}

}