#include <saturn/codegen/serialization_generator.hpp>

#include <mustache/mustache.hpp>
#include <saturn/codegen/utility.hpp>
#include <fstream>

namespace saturn::codegen {

using namespace kainjow;

static void gen_header(VisitResult const& data) {
    std::string tpl = read_file("templates/component_serializers.hpp.mst");
    mustache::mustache must(tpl);

    mustache::data info;
    auto& includes = info["component_includes"] = mustache::data::type::list;
    auto& funcs = info["serializer_declarations"] = mustache::data::type::list;

    for (auto const& component : data.components) {
        includes.push_back({"component_filename", to_filename(component.unscoped_name)});
        funcs.push_back({"component_name", component.name});
    }

    std::string generated = must.render(info);
    std::ofstream out("include/saturn/serialization/component_serializers.hpp");
    out << generated;
}

static void gen_src(VisitResult const& data) {
    std::string tpl = read_file("templates/component_serializers.cpp.mst");
    mustache::mustache must(tpl);

    mustache::data info;

    auto& impls = info["serializer_impl"] = mustache::data::type::list;
    for (auto const& component : data.components) {
        mustache::data component_info;
        component_info["component_name"] = component.name;
        auto& deserialize_fields = component_info["deserialize_field"] = mustache::data::type::list;
        auto& serialize_fields = component_info["serialize_field"] = mustache::data::type::list;
        for (auto const& field : component.fields) {
            if (!(field.flags & FieldFlags::SerializeIgnore)) {
                deserialize_fields.push_back({"field_name", field.name});
                serialize_fields.push_back({"field_name", field.name});
            }
        }
        impls.push_back(component_info);
    }

    auto& entity_info = info["entity_serialize"] = mustache::data::type::list;
    for (auto const& component : data.components) {
        entity_info.push_back({"component_name", component.name});
    }

    std::string generated = must.render(info);
    std::ofstream out("src/serialization/component_serializers.generated.cpp");
    out << generated;
}

void generate_serialization_files(VisitResult const& data) {
    gen_header(data);
    gen_src(data);
}

}