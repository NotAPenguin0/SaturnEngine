#ifndef SATURN_META_INFO_HPP_
#define SATURN_META_INFO_HPP_

#include <string_view>

#include <saturn/meta/type_category.hpp>

namespace saturn::meta {


struct Info {
    std::string_view name;
    // Name without preceding namespace
    std::string_view raw_name;
    // Bit flag representing the category of the type.
    TypeCategory category;
};

} // namespace saturn::meta

#endif