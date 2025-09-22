#ifndef KL_MATH_VECTOR3_HH
#define KL_MATH_VECTOR3_HH

#include <cmath>
#include <concepts>
#include <cstddef>
#include <ostream>
#include <typeinfo>

namespace kl::math {
template <typename T, std::size_t N> struct Vector;

template <std::floating_point T> struct Vector<T, 3> final {
  T x;
  T y;
  T z;

  static constexpr std::size_t kSize = 3;

  static constexpr Vector<T, 3> zero() noexcept {
    return Vector<T, 3>{static_cast<T>(0), static_cast<T>(0),
                        static_cast<T>(0)};
  }

  static constexpr Vector<T, 3> one() noexcept {
    return Vector<T, 3>{static_cast<T>(1), static_cast<T>(1),
                        static_cast<T>(1)};
  }

  static constexpr Vector<T, 3> unitX() noexcept {
    return Vector<T, 3>{static_cast<T>(1), static_cast<T>(0),
                        static_cast<T>(0)};
  }

  static constexpr Vector<T, 3> unitY() noexcept {
    return Vector<T, 3>{static_cast<T>(0), static_cast<T>(1),
                        static_cast<T>(0)};
  }

  static constexpr Vector<T, 3> unitZ() noexcept {
    return Vector<T, 3>{static_cast<T>(0), static_cast<T>(0),
                        static_cast<T>(1)};
  }

  constexpr std::size_t size() const noexcept { return kSize; }

  constexpr float lengthSquared() const noexcept {
    return x * x + y * y + z * z;
  }
  constexpr float length() const noexcept { return std::sqrt(lengthSquared()); }

  constexpr void normalize() noexcept {
    float len = length();
    if (len == 0.0f) {
      (*this) = zero();
      return;
    }
    (*this) /= len;
  }

  static constexpr Vector<T, 3> normalize(const Vector<T, 3> &v) noexcept {
    Vector<T, 3> result = v;
    result.normalize();
    return result;
  }

  static constexpr float dot(const Vector<T, 3> &a,
                             const Vector<T, 3> &b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  static constexpr Vector<T, 3> cross(const Vector<T, 3> &a,
                                      const Vector<T, 3> &b) noexcept {
    return Vector<T, 3>{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x};
  }

  static constexpr float distance(const Vector<T, 3> &a,
                                  const Vector<T, 3> &b) noexcept {
    return (a - b).length();
  }

  static constexpr float distanceSquared(const Vector<T, 3> &a,
                                         const Vector<T, 3> &b) noexcept {
    return (a - b).lengthSquared();
  }

  static constexpr Vector<T, 3> lerp(const Vector<T, 3> &a,
                                     const Vector<T, 3> &b, float t) noexcept {
    return a + t * (b - a);
  }

  constexpr float &operator[](std::size_t index) {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  constexpr Vector<T, 3> &operator+=(const Vector<T, 3> &other) noexcept {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  constexpr Vector<T, 3> &operator-=(const Vector<T, 3> &other) noexcept {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  constexpr Vector<T, 3> &operator*=(float other) noexcept {
    x *= other;
    y *= other;
    z *= other;
    return *this;
  }

  constexpr Vector<T, 3> &operator*=(const Vector<T, 3> &other) noexcept {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
  }

  constexpr Vector<T, 3> &operator/=(float other) noexcept {
    x /= other;
    y /= other;
    z /= other;
    return *this;
  }

  constexpr Vector<T, 3> &operator/=(const Vector<T, 3> &other) noexcept {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
  }

  constexpr Vector<T, 3> operator-() const noexcept {
    return Vector<T, 3>{-x, -y, -z};
  }

  template <typename U>
    requires(std::is_convertible_v<T, U>)
  constexpr operator Vector<U, 3>() const noexcept {
    return Vector<U, 3>{static_cast<U>(x), static_cast<U>(y),
                        static_cast<U>(z)};
  }
};

template <std::floating_point T>
inline constexpr Vector<T, 3> operator+(Vector<T, 3> lhs,
                                        const Vector<T, 3> &rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 3> operator-(Vector<T, 3> lhs,
                                        const Vector<T, 3> &rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 3> operator*(Vector<T, 3> lhs, float rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 3> operator*(float lhs, Vector<T, 3> rhs) noexcept {
  rhs *= lhs;
  return rhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 3> operator*(Vector<T, 3> lhs,
                                        const Vector<T, 3> &rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 3> operator/(Vector<T, 3> lhs, float rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 3> operator/(Vector<T, 3> lhs,
                                        const Vector<T, 3> &rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
inline constexpr Vector<T, 3> operator/(float lhs,
                                        const Vector<T, 3> &rhs) noexcept {
  Vector<T, 3> result{lhs, lhs, lhs};
  result /= rhs;
  return result;
}

template <std::floating_point T>
inline constexpr bool operator==(const Vector<T, 3> &lhs,
                                 const Vector<T, 3> &rhs) noexcept {
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template <std::floating_point T>
inline constexpr bool operator!=(const Vector<T, 3> &lhs,
                                 const Vector<T, 3> &rhs) noexcept {
  return !(lhs == rhs);
}

template <std::floating_point T>
std::ostream &operator<<(std::ostream &os, const Vector<T, 3> &vec) {
  auto name = typeid(T).name();
  os << "Vector<" << name << ", 3>{" << vec.x << ", " << vec.y << ", " << vec.z
     << "}";
  return os;
}

using Vector3 = Vector<float, 3>;
using DVector3 = Vector<double, 3>;
} // namespace kl::math
#endif // KL_MATH_VECTOR3_HH
