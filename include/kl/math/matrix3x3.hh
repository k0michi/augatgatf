#ifndef KL_MATH_MATRIX3X3_HH
#define KL_MATH_MATRIX3X3_HH

#include <optional>

#include "vector3.hh"

namespace kl::math {
template <typename T, std::size_t C, std::size_t R> struct Matrix;

template <std::floating_point T> struct Matrix<T, 3, 3> final {
  union {
    struct {
      Vector<T, 3> col0;
      Vector<T, 3> col1;
      Vector<T, 3> col2;
    };
    struct {
      T m00, m01, m02;
      T m10, m11, m12;
      T m20, m21, m22;
    };
  };

  static constexpr std::size_t kRowSize = 3;
  static constexpr std::size_t kColumnSize = 3;

  constexpr Matrix<T, 3, 3>(const Vector<T, 3> &col0, const Vector<T, 3> &col1,
                            const Vector<T, 3> &col2) noexcept
      : col0(col0), col1(col1), col2(col2) {}
  constexpr Matrix<T, 3, 3>(T m00, T m01, T m02, T m10, T m11, T m12, T m20,
                            T m21, T m22) noexcept
      : m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20),
        m21(m21), m22(m22) {}

  static constexpr Matrix<T, 3, 3> zero() noexcept {
    return Matrix<T, 3, 3>{
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)};
  }

  static constexpr Matrix<T, 3, 3> identity() noexcept {
    return Matrix<T, 3, 3>{
        static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)};
  }

  constexpr std::size_t rowSize() const noexcept { return kRowSize; }

  constexpr std::size_t columnSize() const noexcept { return kColumnSize; }

  constexpr T determinant() const noexcept {
    return m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) +
           m02 * (m10 * m21 - m11 * m20);
  }

  constexpr std::optional<Matrix<T, 3, 3>> inverse() const noexcept {
    T det = determinant();

    if (det == static_cast<T>(0)) {
      return std::nullopt;
    }

    T invDet = static_cast<T>(1) / det;

    return Matrix<T, 3, 3>{
        (m11 * m22 - m12 * m21) * invDet, (m02 * m21 - m01 * m22) * invDet,
        (m01 * m12 - m02 * m11) * invDet, (m12 * m20 - m10 * m22) * invDet,
        (m00 * m22 - m02 * m20) * invDet, (m02 * m10 - m00 * m12) * invDet,
        (m10 * m21 - m11 * m20) * invDet, (m01 * m20 - m00 * m21) * invDet,
        (m00 * m11 - m01 * m10) * invDet};
  }

  constexpr Matrix<T, 3, 3> transpose() noexcept {
    return Matrix<T, 3, 3>{m00, m10, m20, m01, m11, m21, m02, m12, m22};
  };

  constexpr Vector<T, 3> &operator[](std::size_t index) {
    switch (index) {
    case 0:
      return col0;
    case 1:
      return col1;
    case 2:
      return col2;
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  constexpr Matrix<T, 3, 3> &operator+=(const Matrix<T, 3, 3> &other) noexcept {
    col0 += other.col0;
    col1 += other.col1;
    col2 += other.col2;
    return *this;
  }

  constexpr Matrix<T, 3, 3> &operator-=(const Matrix<T, 3, 3> &other) noexcept {
    col0 -= other.col0;
    col1 -= other.col1;
    col2 -= other.col2;
    return *this;
  }

  constexpr Matrix<T, 3, 3> &operator*=(T other) noexcept {
    col0 *= other;
    col1 *= other;
    col2 *= other;
    return *this;
  }

  constexpr Vector<T, 3> operator*(const Vector<T, 3> &vec) const noexcept {
    return Vector<T, 3>{m00 * vec.x + m01 * vec.y + m02 * vec.z,
                        m10 * vec.x + m11 * vec.y + m12 * vec.z,
                        m20 * vec.x + m21 * vec.y + m22 * vec.z};
  }

  constexpr Matrix<T, 3, 3> &operator*=(const Matrix<T, 3, 3> &other) noexcept {
    *this =
        Matrix<T, 3, 3>{m00 * other.m00 + m01 * other.m10 + m02 * other.m20,
                        m00 * other.m01 + m01 * other.m11 + m02 * other.m21,
                        m00 * other.m02 + m01 * other.m12 + m02 * other.m22,
                        m10 * other.m00 + m11 * other.m10 + m12 * other.m20,
                        m10 * other.m01 + m11 * other.m11 + m12 * other.m21,
                        m10 * other.m02 + m11 * other.m12 + m12 * other.m22,
                        m20 * other.m00 + m21 * other.m10 + m22 * other.m20,
                        m20 * other.m01 + m21 * other.m11 + m22 * other.m21,
                        m20 * other.m02 + m21 * other.m12 + m22 * other.m22};
    return *this;
  }

  constexpr Matrix<T, 3, 3> &operator/=(T other) noexcept {
    col0 /= other;
    col1 /= other;
    col2 /= other;
    return *this;
  }

  constexpr Matrix<T, 3, 3> operator-() const noexcept {
    return Matrix<T, 3, 3>{-col0, -col1, -col2};
  }

  template <typename U>
    requires(std::is_convertible_v<T, U>)
  constexpr operator Matrix<U, 3, 3>() const noexcept {
    return Matrix<U, 3, 3>{
        static_cast<U>(m00), static_cast<U>(m01), static_cast<U>(m02),
        static_cast<U>(m10), static_cast<U>(m11), static_cast<U>(m12),
        static_cast<U>(m20), static_cast<U>(m21), static_cast<U>(m22)};
  }
};

template <std::floating_point T>
constexpr Matrix<T, 3, 3> operator+(Matrix<T, 3, 3> lhs,
                                    const Matrix<T, 3, 3> &rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 3, 3> operator-(Matrix<T, 3, 3> lhs,
                                    const Matrix<T, 3, 3> &rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 3, 3> operator*(Matrix<T, 3, 3> lhs, T rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 3, 3> operator*(T lhs, Matrix<T, 3, 3> rhs) noexcept {
  rhs *= lhs;
  return rhs;
}

template <std::floating_point T>
constexpr Matrix<T, 3, 3> operator*(Matrix<T, 3, 3> lhs,
                                    const Matrix<T, 3, 3> &rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 3, 3> operator/(Matrix<T, 3, 3> lhs, T rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 3, 3> operator/(Matrix<T, 3, 3> lhs,
                                    const Matrix<T, 3, 3> &rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
std::ostream &operator<<(std::ostream &os, const Matrix<T, 3, 3> &mat) {
  auto name = typeid(T).name();
  os << "Matrix<" << name << ", 3, 3>{" << mat.m00 << ", " << mat.m01 << ", "
     << mat.m02 << "; " << mat.m10 << ", " << mat.m11 << ", " << mat.m12 << "; "
     << mat.m20 << ", " << mat.m21 << ", " << mat.m22 << "}";
  return os;
}

using Matrix3x3 = Matrix<float, 3, 3>;
using DMatrix3x3 = Matrix<double, 3, 3>;
} // namespace kl::math
#endif // KL_MATH_MATRIX3X3_HH
