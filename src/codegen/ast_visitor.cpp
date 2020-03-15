#include <saturn/codegen/ast_visitor.hpp>

#include <cppast/visitor.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <iostream>

namespace saturn::codegen {

static bool is_component(cppast::cpp_entity const& entity) {
    if (entity.kind() != cppast::cpp_entity_kind::class_t) {
        return false;
    }

    return cppast::has_attribute(entity, "component").has_value();
}

static bool is_field(cppast::cpp_entity const& entity, cppast::visitor_info const& info) {
    return entity.kind() == cppast::cpp_entity_kind::member_variable_t 
           && info.access == cppast::cpp_access_specifier_kind::cpp_public;
}

struct ast_visitor {
    void parse_field(cppast::cpp_entity const& entity) {
        auto& field = cur_component_meta.fields.emplace_back();
        auto const& field_data = static_cast<cppast::cpp_member_variable const&>(entity);
        field.name = field_data.name();
        field.type = cppast::to_string(field_data.type());

        // Check if attributes named 'ignore'. This is essentially serialize::ignore and editor::ignore
        if (auto attr_opt = cppast::has_attribute(entity, "ignore"); attr_opt.has_value()) {
            auto const& attr = attr_opt.value();
            if (auto scope_opt = attr.scope(); scope_opt.has_value()){
                auto const& scope = scope_opt.value();
                if (scope == "serialize") {
                    field.flags |= FieldFlags::SerializeIgnore;
                }
            }
        }
    }
    
    bool operator()(cppast::cpp_entity const& entity, cppast::visitor_info info) {
        if (entity.kind() == cppast::cpp_entity_kind::file_t) {
            // Strip the 'include/' from the filename
            std::string const& raw_name = entity.name();
            filename = raw_name.substr(raw_name.find_first_of('/') + 1);
        }
        // Update current depth if nessecary
        if (info.event == cppast::visitor_info::container_entity_enter) {
            ++depth;
            // Check if the entity is a component and handle it accordingly if nessecary
            if (is_component(entity)) {
                component_depth = depth;
                inside_component = true;
                // Clear old meta
                cur_component_meta = ComponentMeta{};
                cur_component_meta.name = entity.name();
                cur_component_meta.unscoped_name = entity.name();
                cur_component_meta.filename = filename;
            }
        } else if (info.event == cppast::visitor_info::container_entity_exit) {
            --depth;
            if (depth < component_depth && inside_component) {
                // When exiting a component we have to push the gathered meta data into our result struct
                data.components.push_back(cur_component_meta);
                inside_component = false;
                component_depth = 0;
            }
        } else {
            if (is_field(entity, info)) {
                parse_field(entity);
            }
        }

        return true;
    }

    VisitResult data;
    std::string filename = "";
    ComponentMeta cur_component_meta = {};
    bool inside_component = false;
    size_t depth = 0;
    size_t component_depth = 0;
};

std::thread visit_file_ast(std::unique_ptr<cppast::cpp_file> file, VisitResult& result, VisitorSync sync) {
    return std::thread([&result, &sync](std::unique_ptr<cppast::cpp_file> file) {
        ast_visitor visitor;
        cppast::visit(*file, [&visitor](cppast::cpp_entity const& e, cppast::visitor_info info) -> bool {
            return visitor(e, info);
        });
        {
            std::lock_guard results_lock(*sync.results_mutex);
            for (auto& component : visitor.data.components) {
                result.components.push_back(std::move(component));
            }
        }
        std::lock_guard lock(*sync.mutex);
        *sync.thread_counter -= 1;
        sync.cv->notify_all();
    }, std::move(file));
}

}