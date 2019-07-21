#ifndef MVG_IN_RANGE_HPP_
#define MVG_IN_RANGE_HPP_

#include <cassert>
#include <type_traits>

namespace Saturn {

namespace detail {

enum class range { dynamic = 0 };
}

template<auto Min = detail::range::dynamic, auto Max = detail::range::dynamic>
class in_range {
public:
    static_assert(std::is_same<decltype(Min), decltype(Max)>::value,
                  "Min and Max must have the same type");
    // We statically asserted that Min and Max have the same type, so this
    // assumption can be made
    using value_type = decltype(Min);

    in_range(value_type v = value_type{}) : val(v) { verify(); }

    in_range& operator=(value_type v) {
        val = v;
        verify();
        return *this;
    }

    operator value_type() const { return val; }

private:
    void verify() { assert(Min <= val && val <= Max); }

    value_type val;
};

} // namespace Saturn

#endif
