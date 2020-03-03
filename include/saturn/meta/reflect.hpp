#ifndef SATURN_META_REFLECT_HPP_
#define SATURN_META_REFLECT_HPP_

#include <saturn/meta/info.hpp>
#include <saturn/meta/type_name.hpp>

namespace saturn::meta {

template<typename T>
Info reflect() {
    Info info;
    info.name = type_name<T>();
    info.raw_name = type_name_no_namespace<T>();
    info.category = type_category<T>();
    return info;
}

template<typename T>
Info reflect(T const& t) {
    return reflect<T>();
}

} // namespace saturn::meta

#endif