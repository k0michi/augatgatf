#ifndef KL_MATH_MATRIX2X2_HH
#define KL_MATH_MATRIX2X2_HH

#include <optional>

#include "vector2.hh"

namespace kl::math {
template <typename T, std::size_t C, std::size_t R> struct Matrix;

template <> struct Matrix<float, 2, 2> final {
  union {
    struct {
      Vector2 col0;
      Vector2 col1;
    };
    struct {
      float m00, m10;
      float m01, m11;
    };
  };

  static constexpr Matrix<float, 2, 2> zero() noexcept {
    return Matrix<float, 2, 2>{Vector2::zero(), Vector2::zero()};
  }

  static constexpr Matrix<float, 2, 2> identity() noexcept {
    return Matrix<float, 2, 2>{Vector2::unitX(), Vector2::unitY()};
  }

  constexpr float determinant() const noexcept {
    return col0.x * col1.y - col0.y * col1.x;
  }

  constexpr std::optional<Matrix<float, 2, 2>> inverse() const noexcept {
    float det = determinant();

    if (det == 0.0f) {
      return std::nullopt;
    }

    float invDet = 1.0f / det;

    return Matrix<float, 2, 2>{Vector2{col1.y * invDet, -col0.y * invDet},
                               Vector2{-col1.x * invDet, col0.x * invDet}};
  }

  constexpr Matrix<float, 2, 2> transpose() noexcept {
    return Matrix<float, 2, 2>{Vector2{col0.x, col1.x},
                               Vector2{col0.y, col1.y}};
  };

  constexpr Vector2 &operator[](std::size_t index) {
    switch (index) {
    case 0:
      return col0;
    case 1:
      return col1;
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  constexpr Matrix<float, 2, 2> &
  operator+=(const Matrix<float, 2, 2> &other) noexcept {
    col0 += other.col0;
    col1 += other.col1;
    return *this;
  }

  constexpr Matrix<float, 2, 2> &
  operator-=(const Matrix<float, 2, 2> &other) noexcept {
    col0 -= other.col0;
    col1 -= other.col1;
    return *this;
  }

  constexpr Matrix<float, 2, 2> &operator*=(float other) noexcept {
    col0 *= other;
    col1 *= other;
    return *this;
  }

  constexpr Matrix<float, 2, 2> &
  operator*=(const Matrix<float, 2, 2> &other) noexcept {
    *this =
        Matrix<float, 2, 2>{Vector2{col0.x * other.m00 + col1.x * other.m01,
                                    col0.y * other.m00 + col1.y * other.m01},
                            Vector2{col0.x * other.m10 + col1.x * other.m11,
                                    col0.y * other.m10 + col1.y * other.m11}};
    return *this;
  }

  constexpr Matrix<float, 2, 2> &operator/=(float other) noexcept {
    col0 /= other;
    col1 /= other;
    return *this;
  }
};

using Matrix2x2 = Matrix<float, 2, 2>;
using Matrix2 = Matrix<float, 2, 2>;
} // namespace kl::math
#endif // KL_MATH_MATRIX2X2_HH
