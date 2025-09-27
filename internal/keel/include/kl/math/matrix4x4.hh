#ifndef KL_MATH_MATRIX4X4_HH
#define KL_MATH_MATRIX4X4_HH

#include <optional>

#include "vector4.hh"

namespace kl::math {
template <typename T, std::size_t C, std::size_t R> struct Matrix;

template <std::floating_point T> struct Matrix<T, 4, 4> final {
  union {
    struct {
      Vector<T, 4> col0;
      Vector<T, 4> col1;
      Vector<T, 4> col2;
      Vector<T, 4> col3;
    };
    struct {
      T m00, m01, m02, m03;
      T m10, m11, m12, m13;
      T m20, m21, m22, m23;
      T m30, m31, m32, m33;
    };
  };

  static constexpr std::size_t kRowSize = 4;
  static constexpr std::size_t kColumnSize = 4;

  constexpr Matrix<T, 4, 4>(const Vector<T, 4> &col0, const Vector<T, 4> &col1,
                            const Vector<T, 4> &col2,
                            const Vector<T, 4> &col3) noexcept
      : col0(col0), col1(col1), col2(col2), col3(col3) {}

  constexpr Matrix<T, 4, 4>(T m00, T m01, T m02, T m03, T m10, T m11, T m12,
                            T m13, T m20, T m21, T m22, T m23, T m30, T m31,
                            T m32, T m33) noexcept
      : m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12),
        m13(m13), m20(m20), m21(m21), m22(m22), m23(m23), m30(m30), m31(m31),
        m32(m32), m33(m33) {}

  static constexpr Matrix<T, 4, 4> zero() noexcept {
    return Matrix<T, 4, 4>{
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(0)};
  }

  static constexpr Matrix<T, 4, 4> identity() noexcept {
    return Matrix<T, 4, 4>{
        static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(1),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
        static_cast<T>(1)};
  }

  constexpr std::size_t rowSize() const noexcept { return kRowSize; }

  constexpr std::size_t columnSize() const noexcept { return kColumnSize; }

  constexpr T determinant() const noexcept {
    return m00 *
               (m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) +
                m13 * (m21 * m32 - m22 * m31)) -
           m01 *
               (m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30) +
                m13 * (m20 * m32 - m22 * m30)) +
           m02 *
               (m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) +
                m13 * (m20 * m31 - m21 * m30)) -
           m03 *
               (m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) +
                m12 * (m20 * m31 - m21 * m30));
  }

  constexpr std::optional<Matrix<T, 4, 4>> inverse() const noexcept {
    T det = determinant();

    if (det == static_cast<T>(0)) {
      return std::nullopt;
    }

    T invDet = static_cast<T>(1) / det;

    return Matrix<T, 4, 4>{
        (m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) +
         m13 * (m21 * m32 - m22 * m31)) *
            invDet,
        (m02 * (m21 * m33 - m23 * m31) - m01 * (m22 * m33 - m23 * m32) -
         m03 * (m21 * m32 - m22 * m31)) *
            invDet,
        (m01 * (m12 * m33 - m13 * m32) - m02 * (m11 * m33 - m13 * m31) +
         m03 * (m11 * m32 - m12 * m31)) *
            invDet,
        (m02 * (m13 * m21 - m11 * m23) + m03 * (m12 * m21 - m11 * m22) +
         m01 * (m22 * m13 - m12 * m23)) *
            invDet,
        (m12 * (m20 * m33 - m23 * m30) - m10 * (m22 * m33 - m23 * m32) +
         m13 * (m20 * m32 - m22 * m30)) *
            invDet,
        (m00 * (m22 * m33 - m23 * m32) - m02 * (m20 * m33 - m23 * m30) -
         m03 * (m20 * m32 - m22 * m30)) *
            invDet,
        (m02 * (m10 * m33 - m13 * m30) - m00 * (m12 * m33 - m13 * m32) +
         m03 * (m10 * m32 - m12 * m30)) *
            invDet,
        (m00 * (m12 * m23 - m13 * m22) + m03 * (m10 * m22 - m12 * m20) +
         m01 * (m13 * m20 - m10 * m23)) *
            invDet,
        (m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) +
         m13 * (m20 * m31 - m21 * m30)) *
            invDet,
        (m01 * (m20 * m32 - m22 * m30) - m00 * (m21 * m32 - m22 * m31) -
         m03 * (m20 * m31 - m21 * m30)) *
            invDet,
        (m00 * (m11 * m32 - m12 * m31) - m01 * (m10 * m32 - m12 * m30) +
         m03 * (m10 * m31 - m11 * m30)) *
            invDet,
        (m01 * (m12 * m20 - m10 * m22) + m03 * (m10 * m22 - m12 * m20) +
         m02 * (m11 * m20 - m10 * m21)) *
            invDet,
        (m11 * (m20 * m33 - m23 * m30) - m10 * (m21 * m33 - m23 * m31) +
         m13 * (m20 * m31 - m21 * m30)) *
            invDet,
        (m00 * (m21 * m33 - m23 * m31) - m01 * (m20 * m33 - m23 * m30) -
         m03 * (m20 * m31 - m21 * m30)) *
            invDet,
        (m01 * (m10 * m33 - m13 * m30) - m00 * (m11 * m33 - m13 * m31) +
         m03 * (m10 * m31 - m11 * m30)) *
            invDet,
        (m00 * (m11 * m23 - m13 * m21) + m03 * (m10 * m21 - m11 * m20) +
         m01 * (m13 * m20 - m10 * m23)) *
            invDet,
        (m10 * (m22 * m31 - m21 * m32) - m11 * (m20 * m31 - m21 * m30) +
         m12 * (m20 * m32 - m22 * m30)) *
            invDet,
        (m00 * (m21 * m31 - m22 * m30) - m01 * (m20 * m31 - m21 * m30) -
         m02 * (m20 * m32 - m22 * m30)) *
            invDet,
        (m01 * (m12 * m30 - m10 * m32) - m00 * (m11 * m30 - m10 * m31) +
         m02 * (m10 * m31 - m11 * m30)) *
            invDet,
        (m00 * (m12 * m21 - m11 * m22) + m02 * (m10 * m22 - m12 * m20) +
         m01 * (m11 * m20 - m10 * m21)) *
            invDet};
  };

  constexpr Matrix<T, 4, 4> transpose() const noexcept {
    return Matrix<T, 4, 4>{m00, m10, m20, m30, m01, m11, m21, m31,
                           m02, m12, m22, m32, m03, m13, m23, m33};
  };

  constexpr Vector<T, 4> &operator[](std::size_t index) & {
    switch (index) {
    case 0:
      return col0;
    case 1:
      return col1;
    case 2:
      return col2;
    case 3:
      return col3;
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  constexpr const Vector<T, 4> &operator[](std::size_t index) const & {
    switch (index) {
    case 0:
      return col0;
    case 1:
      return col1;
    case 2:
      return col2;
    case 3:
      return col3;
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  constexpr Vector<T, 4> &&operator[](std::size_t index) && {
    switch (index) {
    case 0:
      return std::move(col0);
    case 1:
      return std::move(col1);
    case 2:
      return std::move(col2);
    case 3:
      return std::move(col3);
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  constexpr Matrix<T, 4, 4> &operator+=(const Matrix<T, 4, 4> &other) noexcept {
    col0 += other.col0;
    col1 += other.col1;
    col2 += other.col2;
    col3 += other.col3;
    return *this;
  }

  constexpr Matrix<T, 4, 4> &operator-=(const Matrix<T, 4, 4> &other) noexcept {
    col0 -= other.col0;
    col1 -= other.col1;
    col2 -= other.col2;
    col3 -= other.col3;
    return *this;
  }

  constexpr Matrix<T, 4, 4> &operator*=(T other) noexcept {
    col0 *= other;
    col1 *= other;
    col2 *= other;
    col3 *= other;
    return *this;
  }

  constexpr Matrix<T, 4, 4> &operator*=(const Matrix<T, 4, 4> &other) noexcept {
    *this = Matrix<T, 4, 4>{
        m00 * other.m00 + m01 * other.m10 + m02 * other.m20 + m03 * other.m30,
        m00 * other.m01 + m01 * other.m11 + m02 * other.m21 + m03 * other.m31,
        m00 * other.m02 + m01 * other.m12 + m02 * other.m22 + m03 * other.m32,
        m00 * other.m03 + m01 * other.m13 + m02 * other.m23 + m03 * other.m33,
        m10 * other.m00 + m11 * other.m10 + m12 * other.m20 + m13 * other.m30,
        m10 * other.m01 + m11 * other.m11 + m12 * other.m21 + m13 * other.m31,
        m10 * other.m02 + m11 * other.m12 + m12 * other.m22 + m13 * other.m32,
        m10 * other.m03 + m11 * other.m13 + m12 * other.m23 + m13 * other.m33,
        m20 * other.m00 + m21 * other.m10 + m22 * other.m20 + m23 * other.m30,
        m20 * other.m01 + m21 * other.m11 + m22 * other.m21 + m23 * other.m31,
        m20 * other.m02 + m21 * other.m12 + m22 * other.m22 + m23 * other.m32,
        m20 * other.m03 + m21 * other.m13 + m22 * other.m23 + m23 * other.m33,
        m30 * other.m00 + m31 * other.m10 + m32 * other.m20 + m33 * other.m30,
        m30 * other.m01 + m31 * other.m11 + m32 * other.m21 + m33 * other.m31,
        m30 * other.m02 + m31 * other.m12 + m32 * other.m22 + m33 * other.m32,
        m30 * other.m03 + m31 * other.m13 + m32 * other.m23 + m33 * other.m33};
    return *this;
  }

  constexpr Matrix<T, 4, 4> &operator/=(T other) noexcept {
    col0 /= other;
    col1 /= other;
    col2 /= other;
    col3 /= other;
    return *this;
  }

  constexpr Matrix<T, 4, 4> &operator/=(const Matrix<T, 4, 4> &other) noexcept {
    col0 /= other.col0;
    col1 /= other.col1;
    col2 /= other.col2;
    col3 /= other.col3;
    return *this;
  }

  constexpr Matrix<T, 4, 4> operator-() const noexcept {
    return Matrix<T, 4, 4>{-col0, -col1, -col2, -col3};
  }

  template <typename U>
    requires(std::is_convertible_v<T, U>)
  constexpr operator Matrix<U, 4, 4>() const noexcept {
    return Matrix<U, 4, 4>{
        static_cast<U>(m00), static_cast<U>(m01), static_cast<U>(m02),
        static_cast<U>(m03), static_cast<U>(m10), static_cast<U>(m11),
        static_cast<U>(m12), static_cast<U>(m13), static_cast<U>(m20),
        static_cast<U>(m21), static_cast<U>(m22), static_cast<U>(m23),
        static_cast<U>(m30), static_cast<U>(m31), static_cast<U>(m32),
        static_cast<U>(m33)};
  }

  static constexpr Matrix<T, 4, 4> perspective(T fovY, T aspect, T zNear,
                                               T zFar) noexcept {
    constexpr T f = static_cast<T>(1) / std::tan(fovY / static_cast<T>(2));
    return Matrix<T, 4, 4>{f / aspect,
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           f,
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           (zFar + zNear) / (zNear - zFar),
                           static_cast<T>(-1),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           (static_cast<T>(2) * zFar * zNear) / (zNear - zFar),
                           static_cast<T>(0)};
  }

  static constexpr Matrix<T, 4, 4> frustum(T left, T right, T bottom, T top,
                                           T zNear, T zFar) noexcept {
    return Matrix<T, 4, 4>{(static_cast<T>(2) * zNear) / (right - left),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           (static_cast<T>(2) * zNear) / (top - bottom),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           (right + left) / (right - left),
                           (top + bottom) / (top - bottom),
                           -(zFar + zNear) / (zFar - zNear),
                           static_cast<T>(-1),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear),
                           static_cast<T>(0)};
  };

  static constexpr Matrix<T, 4, 4> ortho(T left, T right, T bottom, T top,
                                         T zNear, T zFar) noexcept {
    return Matrix<T, 4, 4>{static_cast<T>(2) / (right - left),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(2) / (top - bottom),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(0),
                           static_cast<T>(-2) / (zFar - zNear),
                           static_cast<T>(0),
                           -(right + left) / (right - left),
                           -(top + bottom) / (top - bottom),
                           -(zFar + zNear) / (zFar - zNear),
                           static_cast<T>(1)};
  }
};

template <std::floating_point T>
constexpr Matrix<T, 4, 4> operator+(Matrix<T, 4, 4> lhs,
                                    const Matrix<T, 4, 4> &rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 4, 4> operator-(Matrix<T, 4, 4> lhs,
                                    const Matrix<T, 4, 4> &rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 4, 4> operator*(Matrix<T, 4, 4> lhs, T rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 4, 4> operator*(T lhs, Matrix<T, 4, 4> rhs) noexcept {
  rhs *= lhs;
  return rhs;
}

template <std::floating_point T>
constexpr Vector<T, 4> operator*(const Matrix<T, 4, 4> &lhs,
                                 const Vector<T, 4> &rhs) noexcept {
  return Vector<T, 4>{
      lhs.m00 * rhs.x + lhs.m01 * rhs.y + lhs.m02 * rhs.z + lhs.m03 * rhs.w,
      lhs.m10 * rhs.x + lhs.m11 * rhs.y + lhs.m12 * rhs.z + lhs.m13 * rhs.w,
      lhs.m20 * rhs.x + lhs.m21 * rhs.y + lhs.m22 * rhs.z + lhs.m23 * rhs.w,
      lhs.m30 * rhs.x + lhs.m31 * rhs.y + lhs.m32 * rhs.z + lhs.m33 * rhs.w};
}

template <std::floating_point T>
constexpr Vector<T, 4> operator*(const Vector<T, 4> &lhs,
                                 const Matrix<T, 4, 4> &rhs) noexcept {
  return Vector<T, 4>{
      lhs.x * rhs.m00 + lhs.y * rhs.m10 + lhs.z * rhs.m20 + lhs.w * rhs.m30,
      lhs.x * rhs.m01 + lhs.y * rhs.m11 + lhs.z * rhs.m21 + lhs.w * rhs.m31,
      lhs.x * rhs.m02 + lhs.y * rhs.m12 + lhs.z * rhs.m22 + lhs.w * rhs.m32,
      lhs.x * rhs.m03 + lhs.y * rhs.m13 + lhs.z * rhs.m23 + lhs.w * rhs.m33};
}

template <std::floating_point T>
constexpr Matrix<T, 4, 4> operator*(Matrix<T, 4, 4> lhs,
                                    const Matrix<T, 4, 4> &rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 4, 4> operator/(Matrix<T, 4, 4> lhs, T rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Matrix<T, 4, 4> operator/(Matrix<T, 4, 4> lhs,
                                    const Matrix<T, 4, 4> &rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr bool operator==(const Matrix<T, 4, 4> &lhs,
                          const Matrix<T, 4, 4> &rhs) noexcept {
  return lhs.col0 == rhs.col0 && lhs.col1 == rhs.col1 && lhs.col2 == rhs.col2 &&
         lhs.col3 == rhs.col3;
}

template <std::floating_point T>
constexpr bool operator!=(const Matrix<T, 4, 4> &lhs,
                          const Matrix<T, 4, 4> &rhs) noexcept {
  return !(lhs == rhs);
}

template <std::floating_point T>
std::ostream &operator<<(std::ostream &os, const Matrix<T, 4, 4> &matrix) {
  auto name = typeid(T).name();
  os << "Matrix<" << name << ", 4, 4>{" << matrix.m00 << ", " << matrix.m01
     << ", " << matrix.m02 << ", " << matrix.m03 << "; " << matrix.m10 << ", "
     << matrix.m11 << ", " << matrix.m12 << ", " << matrix.m13 << "; "
     << matrix.m20 << ", " << matrix.m21 << ", " << matrix.m22 << ", "
     << matrix.m23 << "; " << matrix.m30 << ", " << matrix.m31 << ", "
     << matrix.m32 << ", " << matrix.m33 << "}";
  return os;
}

using Matrix4x4 = Matrix<float, 4, 4>;
using DMatrix4x4 = Matrix<double, 4, 4>;
} // namespace kl::math
#endif // KL_MATH_MATRIX4X4_HH
