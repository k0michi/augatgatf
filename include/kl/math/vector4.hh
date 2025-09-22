#ifndef KL_MATH_VECTOR4_HH
#define KL_MATH_VECTOR4_HH

#include <cmath>
#include <concepts>
#include <cstddef>
#include <ostream>
#include <typeinfo>

namespace kl::math {
template <typename T, std::size_t N> struct Vector;

template <std::floating_point T> struct Vector<T, 4> final {
  T x;
  T y;
  T z;
  T w;

  static constexpr std::size_t kSize = 4;

  static constexpr Vector<T, 4> zero() noexcept {
    return Vector<T, 4>{T(0), T(0), T(0), T(0)};
  }

  static constexpr Vector<T, 4> one() noexcept {
    return Vector<T, 4>{T(1), T(1), T(1), T(1)};
  }

  static constexpr Vector<T, 4> unitX() noexcept {
    return Vector<T, 4>{T(1), T(0), T(0), T(0)};
  }

  static constexpr Vector<T, 4> unitY() noexcept {
    return Vector<T, 4>{T(0), T(1), T(0), T(0)};
  }

  static constexpr Vector<T, 4> unitZ() noexcept {
    return Vector<T, 4>{T(0), T(0), T(1), T(0)};
  }

  static constexpr Vector<T, 4> unitW() noexcept {
    return Vector<T, 4>{T(0), T(0), T(0), T(1)};
  }

  constexpr std::size_t size() const noexcept { return kSize; }

  constexpr T lengthSquared() const noexcept {
    return x * x + y * y + z * z + w * w;
  }
  constexpr T length() const noexcept { return std::sqrt(lengthSquared()); }

  constexpr void normalize() noexcept {
    T len = length();
    if (len == T(0)) {
      (*this) = zero();
      return;
    }
    (*this) /= len;
  }

  static constexpr Vector<T, 4> normalize(const Vector<T, 4> &v) noexcept {
    Vector<T, 4> result = v;
    result.normalize();
    return result;
  }

  static constexpr T dot(const Vector<T, 4> &a,
                         const Vector<T, 4> &b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
  }

  static constexpr T distance(const Vector<T, 4> &a,
                              const Vector<T, 4> &b) noexcept {
    return (a - b).length();
  }

  static constexpr T distanceSquared(const Vector<T, 4> &a,
                                     const Vector<T, 4> &b) noexcept {
    return (a - b).lengthSquared();
  }

  static constexpr Vector<T, 4> lerp(const Vector<T, 4> &a,
                                     const Vector<T, 4> &b, T t) noexcept {
    return a + t * (b - a);
  }

  constexpr T &operator[](std::size_t index) {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    case 3:
      return w;
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  constexpr Vector<T, 4> &operator+=(const Vector<T, 4> &other) noexcept {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }

  constexpr Vector<T, 4> &operator-=(const Vector<T, 4> &other) noexcept {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }

  constexpr Vector<T, 4> &operator*=(T other) noexcept {
    x *= other;
    y *= other;
    z *= other;
    w *= other;
    return *this;
  }

  constexpr Vector<T, 4> &operator*=(const Vector<T, 4> &other) noexcept {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
    return *this;
  }

  constexpr Vector<T, 4> &operator/=(T other) noexcept {
    x /= other;
    y /= other;
    z /= other;
    w /= other;
    return *this;
  }

  constexpr Vector<T, 4> &operator/=(const Vector<T, 4> &other) noexcept {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;
    return *this;
  }

  constexpr Vector<T, 4> operator-() const noexcept {
    return Vector<T, 4>{-x, -y, -z, -w};
  }

  template <typename U>
    requires(std::is_convertible_v<T, U>)
  constexpr operator Vector<U, 4>() const noexcept {
    return Vector<U, 4>{static_cast<U>(x), static_cast<U>(y), static_cast<U>(z),
                        static_cast<U>(w)};
  }
};

template <std::floating_point T>
inline constexpr Vector<T, 4> operator+(Vector<T, 4> lhs,
                                        const Vector<T, 4> &rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 4> operator-(Vector<T, 4> lhs,
                                        const Vector<T, 4> &rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 4> operator*(Vector<T, 4> lhs, T rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 4> operator*(T lhs, Vector<T, 4> rhs) noexcept {
  rhs *= lhs;
  return rhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 4> operator*(Vector<T, 4> lhs,
                                        const Vector<T, 4> &rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 4> operator/(Vector<T, 4> lhs, T rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 4> operator/(Vector<T, 4> lhs,
                                        const Vector<T, 4> &rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 4> operator/(T lhs,
                                        const Vector<T, 4> &rhs) noexcept {
  Vector<T, 4> result{lhs, lhs, lhs, lhs};
  result /= rhs;
  return result;
}

template <std::floating_point T>
inline constexpr bool operator==(const Vector<T, 4> &lhs,
                                 const Vector<T, 4> &rhs) noexcept {
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

template <std::floating_point T>
inline constexpr bool operator!=(const Vector<T, 4> &lhs,
                                 const Vector<T, 4> &rhs) noexcept {
  return !(lhs == rhs);
}

using Vector4 = Vector<float, 4>;
using DVector4 = Vector<double, 4>;
} // namespace kl::math
#endif // KL_MATH_VECTOR4_HH
