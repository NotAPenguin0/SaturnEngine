#include "Parser.hpp"

#include <cppast/cpp_entity.hpp>
#include <cppast/cpp_entity_index.hpp>
#include <cppast/cpp_file.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <cppast/diagnostic_logger.hpp>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>

namespace impl {

std::string remove_quotes(std::string s) {
    s.erase(std::remove(s.begin(), s.end(), '\"'), s.end());
    return s;
}

struct ast_visitor {
    bool is_interesting(cppast::cpp_entity const& entity) {
        return entity.kind() != cppast::cpp_entity_kind::file_t &&
               !cppast::is_templated(entity) && !cppast::is_friended(entity);
    }

    bool has_attribute(cppast::cpp_entity const& e,

                       std::string_view attr) {

        auto const& attributes = e.attributes();
        return std::find_if(
                   attributes.begin(), attributes.end(),
                   [attr](cppast::cpp_attribute const& attribute) -> bool {
                       return attribute.name() == attr;
                   }) != attributes.end();
    }

    void check_in_component(cppast::cpp_entity const& entity,
                            cppast::visitor_info info) {
        if (info.event == cppast::visitor_info::container_entity_enter) {
            ++cur_depth;

            // If we encounter a struct/class declaration, we should check if
            // it's a component
            if (entity.kind() == cppast::cpp_entity_kind::class_t) {
                // If this entity is a component
                if (auto const& attributes = entity.attributes();
                    has_attribute(entity, "component")) {
                    in_component = true;
                    in_component_child = false;
                    component_depth = cur_depth;
                    // Grab attributes and name
                    data.name = entity.name();
                    if (has_attribute(entity, "default_serialize")) {
                        data.flags |= ComponentData::Flags::DefaultSerialize;
                    }
                    if (has_attribute(entity, "hide_in_editor")) {
                        data.flags |= ComponentData::Flags::HideInEditor;
                    }
                    if (has_attribute(entity, "editor_only")) {
                        data.flags |= ComponentData::Flags::EditorOnly;
                    }
                }
            } else {
                in_component_child = true;
            }
        }

        if (info.event == cppast::visitor_info::container_entity_exit) {
            --cur_depth;
            in_component_child =
                (in_component_child ? (cur_depth != component_depth) : false);
            if (cur_depth < component_depth) { in_component = false; }
        }
    }

    void parse_field_attributes(cppast::cpp_member_variable const& field,
                                ComponentData::FieldMeta& meta) {
        auto const& attributes = field.attributes();
        for (auto const& attribute : attributes) {

            if (attribute.name() == "tooltip") {
                auto const& args = attribute.arguments();
                if (!args) { continue; }
                for (auto const& arg : args.value()) {
                    /*if (arg.kind == cppast::cpp_token_kind::string_literal)
                     * {*/
                    meta.tooltip = remove_quotes(arg.spelling);
                    /*} else {
                        std::cerr << "Invalid argument for attribute "
                                     "[[saturn::tooltip]]. Expected a string, "
                                     "found "
                                  << arg.spelling << "\n";
                    }*/
                }
            }
            if (attribute.name() == "do_not_serialize") {
                meta.do_not_serialize = true;
            }
            if (attribute.name() == "hide_in_editor") {
                meta.hide_in_editor = true;
            }
        }
    }

    bool operator()(cppast::cpp_entity const& entity,
                    cppast::visitor_info info) {
        // Check if this entity kind is interesting for us to parse
        if (!is_interesting(entity)) { return true; }

        if (entity.kind() == cppast::cpp_entity_kind::access_specifier_t) {
            auto const& specifier =
                static_cast<cppast::cpp_access_specifier const&>(entity);
            cur_access = specifier.access_specifier();
        }

        check_in_component(entity, info);
        // Only parse requested fields
        if (!in_component || in_component_child) { return true; }
        if (entity.kind() != cppast::cpp_entity_kind::member_variable_t) {
            return true;
        }

        // Now we have filtered out all entities that are member variables
        // Check if they are public
        auto const& field =
            static_cast<cppast::cpp_member_variable const&>(entity);
        if (cur_access != cppast::cpp_access_specifier_kind::cpp_public) {
            return true;
        }

        ComponentData::FieldMeta meta;
        meta.type = cppast::to_string(field.type());
        parse_field_attributes(field, meta);
        data.fields.insert(std::pair<std::string, ComponentData::FieldMeta>{
            field.name(), meta});

        return true;
    }

    cppast::cpp_access_specifier_kind cur_access =
        cppast::cpp_access_specifier_kind::cpp_public;
    ComponentData data;
    size_t cur_depth = 0;
    size_t component_depth = 0;
    // Specifies whether we should be parsing declarations since we are directly
    // inside a component
    bool in_component = false;
    bool in_component_child = false;
};

ComponentData parse_file(std::unique_ptr<cppast::cpp_file> file) {
    ast_visitor visitor;

    cppast::visit(*file,
                  [&visitor](cppast::cpp_entity const& e,
                             cppast::visitor_info info) { visitor(e, info); });

    return visitor.data;
}

} // namespace impl

ComponentData parse_component_file_new(Directories const& dirs,
                                       std::string_view fname) {
    static cppast::stderr_diagnostic_logger logger;
    cppast::libclang_compile_config config;
    config.set_flags(cppast::cpp_standard::cpp_1z,
                     {cppast::compile_flag::ms_compatibility});
    config.add_include_dir(dirs.include.string());
    config.add_include_dir(dirs.glad.string());
    config.add_include_dir(dirs.glfw.string());
    config.add_include_dir(dirs.glm.string());
    config.add_include_dir(dirs.nlohmann.string());
    config.add_include_dir(dirs.audeo.string());
    config.add_include_dir(dirs.efsw.string());
    cppast::cpp_entity_index idx;
    cppast::libclang_parser parser(type_safe::ref(logger));
    std::unique_ptr<cppast::cpp_file> file =
        parser.parse(idx, std::string(fname), config);

    return impl::parse_file(std::move(file));
}
