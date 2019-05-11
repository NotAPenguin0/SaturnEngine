#ifndef MVG_MATRIX_HPP_
#define MVG_MATRIX_HPP_

#include <array>
#include <cassert>
#include <type_traits>
#include <utility>

namespace Saturn::Math {

template<typename T, std::size_t R, std::size_t C>
class Matrix;

template<typename T>
class Matrix<T, 3, 3> {
public:
    static constexpr std::size_t Width = 3;
    static constexpr std::size_t Height = 3;

    static_assert(
        std::is_floating_point<T>::value,
        "Saturn::Math: Matrix value type must be floating point type");

    constexpr Matrix() : data_() {}
    constexpr Matrix(std::array<std::array<T, Height>, Width> const& data) :
        data_(data) {}
    constexpr Matrix(Matrix const& rhs) : data_(rhs.data_) {}
    constexpr Matrix(Matrix&& rhs) : data_(std::move(rhs.data_)) {}

    constexpr Matrix& operator=(Matrix const& rhs) {
        data_ = rhs.data_;
        return *this;
    }

    constexpr Matrix& operator=(Matrix&& rhs) {
        data_ = std::move(rhs.data_);
        return *this;
    }

    // accessors

    constexpr T& at(std::size_t x, std::size_t y) {
        assert(x <= Width && y <= Height);
        return data_[x][y];
    }

    constexpr T const& at(std::size_t x, std::size_t y) const {
        assert(x <= Width && y <= Height);
        return data_[x][y];
    }

    constexpr T& operator()(std::size_t x, std::size_t y) {
        return data_[x][y];
    }

    constexpr T const& operator()(std::size_t x, std::size_t y) const {
        return data_[x][y];
    }

    constexpr auto begin() { return data_.begin(); }
    constexpr auto begin() const { return data_.begin(); }
    constexpr auto end() { return data_.end(); }
    constexpr auto end() const { return data_.end(); }

    static constexpr Matrix identity() {
        Matrix result;
        for (std::size_t y = 0; y < Height; ++y) {
            for (std::size_t x = 0; x < Width; ++x) {
                if (x == y) {
                    result(x, y) = 1.0;
                } else {
                    result(x, y) = 0;
                }
            }
        }
        return result;
    }

    // Operations

    constexpr Matrix transpose() const {
        Matrix result;
        for (std::size_t y = 0; y < Height; ++y) {
            for (std::size_t x = 0; x < Width; ++x) { result(x, y) = at(y, x); }
        }
    }

    constexpr T determinant() const {
        // yeah lol
        return data_[0][0] * data_[1][1] * data_[2][2] +
               data_[0][1] * data_[1][2] * data_[2][0] +
               data_[0][2] * data_[1][0] * data_[2][1] -
               (data_[0][0] * data_[1][2] * data_[2][1] +
                data_[0][1] * data_[2][0] * data_[2][2] +
                data_[0][2] * data_[1][1] * data_[2][0]);
    }

    constexpr T* ptr() { return &data_[0][0]; }
    constexpr T const* ptr() const { return &data_[0][0]; }

private:
    std::array<std::array<T, Height>, Width> data_;
};

template<typename T>
class Matrix<T, 4, 4> {
public:
    static constexpr std::size_t Width = 4;
    static constexpr std::size_t Height = 4;

    static_assert(
        std::is_floating_point<T>::value,
        "Saturn::Math: Matrix value type must be floating point type");

    constexpr Matrix() : data_() {}
    constexpr Matrix(std::array<std::array<T, Height>, Width> const& data) :
        data_(data) {}
    constexpr Matrix(Matrix const& rhs) : data_(rhs.data_) {}
    constexpr Matrix(Matrix&& rhs) : data_(std::move(rhs.data_)) {}

    constexpr Matrix& operator=(Matrix const& rhs) {
        data_ = rhs.data_;
        return *this;
    }

    constexpr Matrix& operator=(Matrix&& rhs) {
        data_ = std::move(rhs.data_);
        return *this;
    }

    // accessors

    constexpr T& at(std::size_t x, std::size_t y) {
        assert(x <= Width && y <= Height);
        return data_[x][y];
    }

    constexpr T const& at(std::size_t x, std::size_t y) const {
        assert(x <= Width && y <= Height);
        return data_[x][y];
    }

    constexpr T& operator()(std::size_t x, std::size_t y) {
        return data_[x][y];
    }

    constexpr T const& operator()(std::size_t x, std::size_t y) const {
        return data_[x][y];
    }

    constexpr auto begin() { return data_.begin(); }
    constexpr auto begin() const { return data_.begin(); }
    constexpr auto end() { return data_.end(); }
    constexpr auto end() const { return data_.end(); }

    static constexpr Matrix identity() {
        Matrix result;
        for (std::size_t y = 0; y < Height; ++y) {
            for (std::size_t x = 0; x < Width; ++x) {
                if (x == y) {
                    result(x, y) = 1.0;
                } else {
                    result(x, y) = 0;
                }
            }
        }
        return result;
    }

    // Operations

    constexpr Matrix<T, 3, 3> get_minor(std::size_t col_to_slice,
                                        std::size_t row_to_slice) const {
        Matrix<T, 3, 3> result;
        for (std::size_t y = 0; y < Height; ++y) {
            for (std::size_t x = 0; x < Width; ++x) {
                if (y != row_to_slice && x != col_to_slice) {
                    auto x_idx = (x < col_to_slice) ? x : x - 1;
                    auto y_idx = (y < row_to_slice) ? y : y - 1;
                    result(x_idx, y_idx) = at(x, y);
                }
            }
        }
        return result;
    }

    constexpr Matrix transpose() const {
        Matrix result;
        for (std::size_t y = 0; y < Height; ++y) {
            for (std::size_t x = 0; x < Width; ++x) { result(x, y) = at(y, x); }
        }
    }

    constexpr T determinant() const {
        T result{};
        T factor = 1;
        for (std::size_t x = 0; x < Width; ++x) {
            result += (data_[0][x] * get_minor(x, 0).determinant() * factor);
            factor *= -1;
        }
        return result;
    }

    constexpr T* ptr() { return &data_[0][0]; }
    constexpr T const* ptr() const { return &data_[0][0]; }

private:
    std::array<std::array<T, Height>, Width> data_;

}; // namespace Saturn::Math

template<typename T>
using Matrix3x3 = Matrix<T, 3, 3>;

template<typename T>
using Matrix4x4 = Matrix<T, 4, 4>;

template<typename T,
         std::size_t W1,
         std::size_t H1,
         std::size_t W2,
         std::size_t H2>
constexpr Matrix<T, W1, H2> operator*(Matrix<T, W1, H1> const& lhs,
                                      Matrix<T, W2, H2> const& rhs) {
    static_assert(H1 == W2, "Invalid matrix multiplication");

    Matrix<T, W2, H1> result;
    for (size_t x = 0; x < W2; x++) {
        for (size_t y = 0; y < H1; y++) {
            T dot{};
            for (size_t i = 0; i < H1; i++)
                dot += lhs(i, y) * rhs(x, i);
            result(x, y) = dot;
        }
    }
    return result;
}


} // namespace Saturn::Math

#endif
