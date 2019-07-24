#ifndef SATURN_ID_GENERATOR_HPP_
#define SATURN_ID_GENERATOR_HPP_

#include <cstddef>

namespace Saturn {

template<typename T>
class IDGenerator {
public:
    static std::size_t next() { return ++cur; }

    static constexpr inline std::size_t none = 0;

private:
    static inline std::size_t cur = 0;
};

} // namespace Saturn

#endif
