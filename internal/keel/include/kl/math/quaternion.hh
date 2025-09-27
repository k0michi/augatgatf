#ifndef KL_MATH_QUATERNION_HH
#define KL_MATH_QUATERNION_HH

#include <cmath>
#include <concepts>
#include <cstddef>
#include <ostream>
#include <typeinfo>

#include "vector3.hh"

namespace kl::math {
template <typename T> struct Quaternion;

template <std::floating_point T> struct Quaternion<T> final {
  T x;
  T y;
  T z;
  T w;

  static constexpr std::size_t kSize = 4;

  static constexpr Quaternion<T> identity() noexcept {
    return Quaternion<T>{static_cast<T>(0), static_cast<T>(0),
                         static_cast<T>(0), static_cast<T>(1)};
  }

  static constexpr Quaternion<T> fromAxisAngle(const Vector<T, 3> &axis,
                                               T angle) noexcept {
    T halfAngle = angle / static_cast<T>(2);
    T s = std::sin(halfAngle);
    T c = std::cos(halfAngle);
    return Quaternion<T>{axis.x * s, axis.y * s, axis.z * s, c};
  }

  constexpr std::size_t size() const noexcept { return kSize; }

  constexpr static T dot(const Quaternion<T> &a,
                         const Quaternion<T> &b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
  }

  constexpr T lengthSquared() const noexcept { return dot(*this, *this); }

  constexpr T length() const noexcept { return std::sqrt(lengthSquared()); }

  constexpr void normalize() noexcept {
    T len = length();
    (*this) /= len;
  }

  static constexpr Quaternion<T> normalize(const Quaternion<T> &q) noexcept {
    Quaternion<T> result = q;
    result.normalize();
    return result;
  }

  constexpr Quaternion<T> conjugate() const noexcept {
    return Quaternion<T>{-x, -y, -z, w};
  }

  constexpr std::optional<Quaternion<T>> inverse() const noexcept {
    T lenSq = lengthSquared();

    if (lenSq == static_cast<T>(0)) {
      return std::nullopt;
    }

    T invLenSq = static_cast<T>(1) / lenSq;
    return Quaternion<T>{-x * invLenSq, -y * invLenSq, -z * invLenSq,
                         w * invLenSq};
  }

  constexpr Quaternion<T> &operator*=(const Quaternion<T> &other) noexcept {
    T newX = w * other.x + x * other.w + y * other.z - z * other.y;
    T newY = w * other.y - x * other.z + y * other.w + z * other.x;
    T newZ = w * other.z + x * other.y - y * other.x + z * other.w;
    T newW = w * other.w - x * other.x - y * other.y - z * other.z;
    x = newX;
    y = newY;
    z = newZ;
    w = newW;
    return *this;
  }

  constexpr Quaternion<T> &operator*=(T scalar) noexcept {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  }

  constexpr Quaternion<T> &operator/=(const Quaternion<T> &other) {
    auto inv = other.inverse();

    if (!inv) {
      throw std::runtime_error("Division by zero quaternion");
    }

    return (*this) *= *inv;
  }

  constexpr Quaternion<T> &operator/=(T scalar) noexcept {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
  }

  constexpr Quaternion<T> &operator+=(const Quaternion<T> &other) noexcept {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }

  constexpr Quaternion<T> &operator-=(const Quaternion<T> &other) noexcept {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }

  constexpr T &operator[](std::size_t index) & {
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

  constexpr const T &operator[](std::size_t index) const & {
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

  constexpr T &&operator[](std::size_t index) && {
    switch (index) {
    case 0:
      return std::move(x);
    case 1:
      return std::move(y);
    case 2:
      return std::move(z);
    case 3:
      return std::move(w);
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  constexpr Quaternion<T> &operator-() noexcept {
    x = -x;
    y = -y;
    z = -z;
    w = -w;
    return *this;
  }

  constexpr Quaternion<T> operator-() const noexcept {
    return Quaternion<T>{-x, -y, -z, -w};
  }

  constexpr bool operator==(const Quaternion<T> &other) const noexcept {
    return x == other.x && y == other.y && z == other.z && w == other.w;
  }

  constexpr bool operator!=(const Quaternion<T> &other) const noexcept {
    return !(*this == other);
  }
};

template <std::floating_point T>
constexpr Quaternion<T> operator+(Quaternion<T> lhs,
                                  const Quaternion<T> &rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Quaternion<T> operator-(Quaternion<T> lhs,
                                  const Quaternion<T> &rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Quaternion<T> operator*(Quaternion<T> lhs,
                                  const Quaternion<T> &rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Quaternion<T> operator*(Quaternion<T> lhs, T rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Quaternion<T> operator*(T lhs, Quaternion<T> rhs) noexcept {
  rhs *= lhs;
  return rhs;
}

template <std::floating_point T>
constexpr Quaternion<T> operator/(T lhs, const Quaternion<T> &rhs) noexcept {
  Quaternion<T> result{lhs, lhs, lhs, lhs};
  result /= rhs;
  return result;
}

template <std::floating_point T>
constexpr Quaternion<T> operator/(Quaternion<T> lhs, T rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Quaternion<T> operator/(Quaternion<T> lhs,
                                  const Quaternion<T> &rhs) noexcept {
  lhs /= rhs;
  return lhs;
}
} // namespace kl::math
#endif // KL_MATH_QUATERNION_HH