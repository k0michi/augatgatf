#ifndef KL_MATH_MATRIX2X2_HH
#define KL_MATH_MATRIX2X2_HH

#include <optional>

#include "vector2.hh"

namespace kl::math {
template <typename T, std::size_t C, std::size_t R> struct Matrix;

template <std::floating_point T> struct Matrix<T, 2, 2> final {
  union {
    struct {
      Vector<T, 2> col0;
      Vector<T, 2> col1;
    };
    struct {
      T m00, m01;
      T m10, m11;
    };
  };

  static constexpr std::size_t kRowSize = 2;
  static constexpr std::size_t kColmunSize = 2;

  constexpr Matrix<T, 2, 2>(const Vector<T, 2> &col0,
                            const Vector<T, 2> &col1) noexcept
      : col0(col0), col1(col1) {}
  constexpr Matrix<T, 2, 2>(T m00, T m01, T m10, T m11) noexcept
      : m00(m00), m01(m01), m10(m10), m11(m11) {}

  static constexpr Matrix<T, 2, 2> zero() noexcept {
    return Matrix<T, 2, 2>{Vector2::zero(), Vector2::zero()};
  }

  static constexpr Matrix<T, 2, 2> identity() noexcept {
    return Matrix<T, 2, 2>{Vector2::unitX(), Vector2::unitY()};
  }

  constexpr std::size_t rowSize() const noexcept { return kRowSize; }

  constexpr std::size_t columnSize() const noexcept { return kColmunSize; }

  constexpr T determinant() const noexcept { return m00 * m11 - m01 * m10; }

  constexpr std::optional<Matrix<T, 2, 2>> inverse() const noexcept {
    T det = determinant();

    if (det == static_cast<T>(0)) {
      return std::nullopt;
    }

    T invDet = static_cast<T>(1) / det;

    return Matrix<T, 2, 2>{m11 * invDet, -m01 * invDet, -m10 * invDet,
                           m00 * invDet};
  }

  constexpr Matrix<T, 2, 2> transpose() noexcept {
    return Matrix<T, 2, 2>{m00, m10, m01, m11};
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

  constexpr Matrix<T, 2, 2> &operator+=(const Matrix<T, 2, 2> &other) noexcept {
    col0 += other.col0;
    col1 += other.col1;
    return *this;
  }

  constexpr Matrix<T, 2, 2> &operator-=(const Matrix<T, 2, 2> &other) noexcept {
    col0 -= other.col0;
    col1 -= other.col1;
    return *this;
  }

  constexpr Matrix<T, 2, 2> &operator*=(T other) noexcept {
    col0 *= other;
    col1 *= other;
    return *this;
  }

  constexpr Vector<T, 2> operator*(const Vector<T, 2> &vec) const noexcept {
    return Vector<T, 2>{m00 * vec.x + m01 * vec.y, m10 * vec.x + m11 * vec.y};
  }

  constexpr Matrix<T, 2, 2> &operator*=(const Matrix<T, 2, 2> &other) noexcept {
    *this = Matrix<T, 2, 2>{
        m00 * other.m00 + m01 * other.m10, m00 * other.m01 + m01 * other.m11,
        m10 * other.m00 + m11 * other.m10, m10 * other.m01 + m11 * other.m11};
    return *this;
  }

  constexpr Matrix<T, 2, 2> &operator/=(T other) noexcept {
    col0 /= other;
    col1 /= other;
    return *this;
  }

  constexpr Matrix<T, 2, 2> &operator/=(const Matrix<T, 2, 2> &other) noexcept {
    col0 /= other.col0;
    col1 /= other.col1;
    return *this;
  }

  constexpr Matrix<T, 2, 2> operator-() const noexcept {
    return Matrix<T, 2, 2>{-col0, -col1};
  }

  template <typename U>
    requires(std::is_convertible_v<T, U>)
  constexpr operator Matrix<U, 2, 2>() const noexcept {
    return Matrix<U, 2, 2>{static_cast<U>(m00), static_cast<U>(m01),
                           static_cast<U>(m10), static_cast<U>(m11)};
  }
};

template <std::floating_point T>
constexpr Matrix<T, 2, 2> operator+(Matrix<T, 2, 2> lhs,
                                    const Matrix<T, 2, 2> &rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 2, 2> operator-(Matrix<T, 2, 2> lhs,
                                    const Matrix<T, 2, 2> &rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 2, 2> operator*(Matrix<T, 2, 2> lhs, T rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 2, 2> operator*(T lhs, Matrix<T, 2, 2> rhs) noexcept {
  rhs *= lhs;
  return rhs;
}

template <std::floating_point T>
constexpr Matrix<T, 2, 2> operator*(Matrix<T, 2, 2> lhs,
                                    const Matrix<T, 2, 2> &rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 2, 2> operator/(Matrix<T, 2, 2> lhs, T rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 2, 2> operator/(Matrix<T, 2, 2> lhs,
                                    const Matrix<T, 2, 2> &rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
std::ostream &operator<<(std::ostream &os, const Matrix<T, 2, 2> &mat) {
  auto name = typeid(T).name();
  os << "Matrix<" << name << ", 2, 2>{" << mat.m00 << ", " << mat.m01 << "; "
     << mat.m10 << ", " << mat.m11 << "}";
  return os;
}

using Matrix2x2 = Matrix<float, 2, 2>;
using Matrix2 = Matrix<float, 2, 2>;
} // namespace kl::math
#endif // KL_MATH_MATRIX2X2_HH
