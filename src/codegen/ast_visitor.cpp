#include <saturn/codegen/ast_visitor.hpp>

#include <cppast/visitor.hpp>
#include <iostream>

namespace saturn::codegen {

bool is_component(cppast::cpp_entity const& entity) {
    if (entity.kind() != cppast::cpp_entity_kind::class_t) {
        return false;
    }

    return cppast::has_attribute(entity, "component").has_value();
}

struct ast_visitor {
    bool operator()(cppast::cpp_entity const& entity, cppast::visitor_info info) {
        
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
            }
        } else if (info.event == cppast::visitor_info::container_entity_exit) {
            --depth;
            if (depth < component_depth && inside_component) {
                // When exiting a component we have to push the gathered meta data into our result struct
                data.components.push_back(cur_component_meta);
                inside_component = false;
                component_depth = 0;
            }
        }

        

        return true;
    }

    VisitResult data;

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