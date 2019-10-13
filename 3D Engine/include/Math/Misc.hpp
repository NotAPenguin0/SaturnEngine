#ifndef SATURN_MISC_HPP_
#define SATURN_MISC_HPP_

namespace Saturn::Math {
template<typename T>
T map_range(T input, T input_start, T input_end, T output_start, T output_end) {
    const T slope = (output_end - output_start) / (input_end - input_start);
    T output = output_start + slope * (input - input_start);
    return output;
}

} // namespace Saturn::Math

#endif
