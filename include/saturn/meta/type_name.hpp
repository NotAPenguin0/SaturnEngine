#ifndef SATURN_META_TYPE_NAME_HPP_
#define SATURN_META_TYPE_NAME_HPP_

#include <string>
#include <string_view>

// Note: This header currently only supports clang.

namespace saturn::meta {

template<typename T>
std::string_view type_name() {
#ifndef MSVC_VER
    // Example returned string: std::string saturn::meta::type_name() [T = saturn::components::Transform]
    // Note that here, the actual type name starts at index 50
    static std::string pretty_func = std::string(__PRETTY_FUNCTION__);
    static std::string result = pretty_func.substr(50, pretty_func.size() - 51);
    return result;
#else
    // Not implemented
    return __FUNCSIG__;
#endif
}

template<typename T>
std::string_view type_name_no_namespace() {
#ifndef MSVC_VER
    static std::string const& full_name = type_name<T>();
    static size_t const last_colon_index = full_name.find_last_of(':');
    static std::string no_namespace = full_name.substr(last_colon_index + 1);
    return no_namespace;
#else
    // Not implemented
    return __FUNCSIG__;
#endif
}

} // namespace saturn::meta

#endif