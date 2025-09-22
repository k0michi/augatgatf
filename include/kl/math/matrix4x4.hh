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
      : m00(m00), m01(m01), m02(m02), m03
};
} // namespace kl::math
#endif // KL_MATH_MATRIX4X4_HH
