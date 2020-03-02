#ifndef SATURN_STL_BIT_FLAG_HPP_
#define SATURN_STL_BIT_FLAG_HPP_

#include <saturn/stl/traits.hpp>

namespace stl {

// Represents a type safe bit flag
template<typename E>
class bit_flag {
public:
    using enum_type = E;
    using underlying_type = underlying_type_t<E>;

    bit_flag() = default;
    bit_flag(underlying_type value) : _value(value) {}
    bit_flag(bit_flag const&) = default;
    bit_flag& operator=(bit_flag const&) = default;

    underlying_type value() const {
        return _value;
    }

    explicit operator underlying_type() const {
        return _value;
    }

    explicit operator bool() const {
        return static_cast<bool>(_value);
    }

    // Operations

    friend bit_flag operator&(bit_flag const lhs, bit_flag const rhs) {
        return bit_flag(lhs._value & rhs._value);
    }

    friend bit_flag operator|(bit_flag const lhs, bit_flag const& rhs) {
        return bit_flag(lhs._value | rhs._value);
    }

    friend bit_flag operator^(bit_flag const lhs, bit_flag const rhs) {
        return bit_flag(lhs._value ^ rhs.v_alue);
    }

    friend bit_flag operator~(bit_flag const lhs) {
        return bit_flag(~lhs._value);
    }

    friend bit_flag operator&(bit_flag const lhs, E const rhs) {
        return bit_flag(lhs._value & static_cast<underlying_type>(rhs));
    }

    friend bit_flag operator|(bit_flag const lhs, E const& rhs) {
        return bit_flag(lhs._value | static_cast<underlying_type>(rhs));
    }

    friend bit_flag operator^(bit_flag const lhs, E const rhs) {
        return bit_flag(lhs._value ^ static_cast<underlying_type>(rhs));
    }

    friend bit_flag operator&(E const lhs, bit_flag const rhs) {
         return bit_flag(static_cast<underlying_type>(lhs) & rhs._value);
    }

    friend bit_flag operator|(E const lhs, bit_flag const& rhs) {
         return bit_flag(static_cast<underlying_type>(lhs) | rhs._value);
    }

    friend bit_flag operator^(E const lhs, bit_flag const rhs) {
        return bit_flag(static_cast<underlying_type>(lhs) ^ rhs._value);
    }

    friend bool operator!(bit_flag const lhs) {
        return !lhs._value;
    }

    friend bool operator==(bit_flag const lhs, bit_flag const rhs) {
        return lhs._value == rhs._value;
    }

    friend bool operator!=(bit_flag const lhs, bit_flag const rhs) {
        return lhs._value != rhs._value;
    }

    bit_flag& operator&=(bit_flag const rhs) {
        _value &= rhs._value;
        return *this;
    }

    bit_flag& operator|=(bit_flag const rhs) {
        _value |= rhs._value;
        return *this;
    }

    bit_flag& operator^=(bit_flag const rhs) {
        _value ^= rhs._value;
        return *this;
    }

    bit_flag& operator&=(E const rhs) {
        _value &= static_cast<underlying_type>(rhs);
        return *this;
    }

    bit_flag& operator|=(E const rhs) {
        _value |= static_cast<underlying_type>(rhs);
        return *this;
    }

    bit_flag& operator^=(E const rhs) {
        _value ^= static_cast<underlying_type>(rhs);
        return *this;
    }

private:
    underlying_type _value = 0;
}; 

}

#endif