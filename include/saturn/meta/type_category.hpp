#ifndef SATURN_TYPE_CATEGORY_HPP_
#define SATURN_TYPE_CATEGORY_HPP_

#include <saturn/stl/types.hpp>
#include <saturn/stl/bit_flag.hpp>

namespace saturn::meta {

class TypeCategory : public stl::bit_flag<stl::uint32_t> {
public:
    using bit_flag::bit_flag;
    
    static constexpr stl::uint32_t Component = 1;
    static constexpr stl::uint32_t System = 2;
    static constexpr stl::uint32_t Unknown = 4;
};

template<typename T>
TypeCategory type_category();

}

#endif