#ifndef SATURN_META_INFO_HPP_
#define SATURN_META_INFO_HPP_

#include <string_view>

namespace saturn::meta {


struct Info {
    std::string_view name;
    // Name without preceding namespace
    std::string_view raw_name;
};

} // namespace saturn::meta

#endif