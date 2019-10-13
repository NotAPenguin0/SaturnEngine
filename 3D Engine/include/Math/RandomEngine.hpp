#ifndef SATURN_RANDOM_ENGINE_HPP_
#define SATURN_RANDOM_ENGINE_HPP_

#include <mutex>
#include <random>
#include <type_traits>

namespace Saturn {

namespace Math {

class RandomEngine {
public:
    static void initialize();

    template<typename T>
    static T get(T min, T max) {
        static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");

        if constexpr (std::is_integral_v<T>) {
            // Use std::uniform_int_distribution for integral types
            std::uniform_int_distribution<T> distr(min, max);
            std::lock_guard<std::mutex> lock(mut);
            return distr(engine);
        } else if constexpr (std::is_floating_point_v<T>) {
            // Use std::uniform_real_distribution for floating point types
            std::uniform_real_distribution<T> distr(min, max);
            std::lock_guard<std::mutex> lock(mut);
            return distr(engine);
        }

        throw std::runtime_error(
            "Code execution reached a path that should never be reached");
    }

private:
    static inline std::random_device rd;
    static inline std::mt19937 engine;
    static inline std::mutex mut;
};

} // namespace Math

} // namespace Saturn

#endif
