#ifndef KL_MATH_VECTOR2_HH
#define KL_MATH_VECTOR2_HH

#include <cmath>
#include <concepts>
#include <ostream>
#include <stdexcept>
#include <typeinfo>

namespace kl::math {
template <typename T, std::size_t N> struct Vector;

template <std::floating_point T>
constexpr Vector<T, 2> operator+(Vector<T, 2> lhs,
                                 const Vector<T, 2> &rhs) noexcept;
template <std::floating_point T>
constexpr Vector<T, 2> operator-(Vector<T, 2> lhs,
                                 const Vector<T, 2> &rhs) noexcept;
template <std::floating_point T>
constexpr Vector<T, 2> operator*(T lhs, Vector<T, 2> rhs) noexcept;

template <std::floating_point T> struct Vector<T, 2> final {
  T x;
  T y;

  static constexpr Vector<T, 2> zero() noexcept {
    return Vector<T, 2>{0.0f, 0.0f};
  }

  static constexpr Vector<T, 2> one() noexcept {
    return Vector<T, 2>{1.0f, 1.0f};
  }

  static constexpr Vector<T, 2> unitX() noexcept {
    return Vector<T, 2>{1.0f, 0.0f};
  }

  static constexpr Vector<T, 2> unitY() noexcept {
    return Vector<T, 2>{0.0f, 1.0f};
  }

  constexpr T lengthSquared() const noexcept { return x * x + y * y; }

  constexpr T length() const noexcept { return std::sqrt(lengthSquared()); }

  constexpr void normalize() noexcept {
    T len = length();

    if (len == 0.0f) {
      (*this) = zero();
      return;
    }

    (*this) /= len;
  }

  static constexpr Vector<T, 2> normalize(const Vector<T, 2> &v) noexcept {
    Vector<T, 2> result = v;
    result.normalize();
    return result;
  }

  static constexpr T dot(const Vector<T, 2> &a,
                         const Vector<T, 2> &b) noexcept {
    return a.x * b.x + a.y * b.y;
  }

  static constexpr T cross(const Vector<T, 2> &a,
                           const Vector<T, 2> &b) noexcept {
    return a.x * b.y - a.y * b.x;
  }

  static constexpr T distance(const Vector<T, 2> &a,
                              const Vector<T, 2> &b) noexcept {
    return (a - b).length();
  }

  static constexpr T distanceSquared(const Vector<T, 2> &a,
                                     const Vector<T, 2> &b) noexcept {
    return (a - b).lengthSquared();
  }

  static constexpr Vector<T, 2> lerp(const Vector<T, 2> &a,
                                     const Vector<T, 2> &b, T t) noexcept {
    return a + t * (b - a);
  }

  constexpr T &operator[](std::size_t index) {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  constexpr Vector<T, 2> &operator+=(const Vector<T, 2> &other) noexcept {
    x += other.x;
    y += other.y;
    return *this;
  }

  constexpr Vector<T, 2> &operator-=(const Vector<T, 2> &other) noexcept {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  constexpr Vector<T, 2> &operator*=(T other) noexcept {
    x *= other;
    y *= other;
    return *this;
  }

  constexpr Vector<T, 2> &operator*=(const Vector<T, 2> &other) noexcept {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  constexpr Vector<T, 2> &operator/=(T other) noexcept {
    x /= other;
    y /= other;
    return *this;
  }

  constexpr Vector<T, 2> &operator/=(const Vector<T, 2> &other) noexcept {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  constexpr Vector<T, 2> operator-() const noexcept {
    return Vector<T, 2>{-x, -y};
  }
};

template <std::floating_point T>
constexpr Vector<T, 2> operator+(Vector<T, 2> lhs,
                                 const Vector<T, 2> &rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Vector<T, 2> operator-(Vector<T, 2> lhs,
                                 const Vector<T, 2> &rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Vector<T, 2> operator*(Vector<T, 2> lhs, float rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Vector<T, 2> operator*(float lhs, Vector<T, 2> rhs) noexcept {
  rhs *= lhs;
  return rhs;
}

template <std::floating_point T>
constexpr Vector<T, 2> operator*(Vector<T, 2> lhs,
                                 const Vector<T, 2> &rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Vector<T, 2> operator/(Vector<T, 2> lhs, T rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Vector<T, 2> operator/(Vector<T, 2> lhs,
                                 const Vector<T, 2> &rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template <std::floating_point T>
constexpr Vector<T, 2> operator/(T lhs, const Vector<T, 2> &rhs) noexcept {
  Vector<T, 2> result{lhs, lhs};
  result /= rhs;
  return result;
}

template <std::floating_point T>
constexpr bool operator==(const Vector<T, 2> &lhs,
                          const Vector<T, 2> &rhs) noexcept {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <std::floating_point T>
constexpr bool operator!=(const Vector<T, 2> &lhs,
                          const Vector<T, 2> &rhs) noexcept {
  return !(lhs == rhs);
}

template <std::floating_point T>
std::ostream &operator<<(std::ostream &os, const Vector<T, 2> &vec) {
  auto name = typeid(T).name();
  os << "Vector<" << name << ", 2>{" << vec.x << ", " << vec.y << "}";
  return os;
}

using Vector2 = Vector<float, 2>;
using DVector2 = Vector<double, 2>;
} // namespace kl::math
#endif // KL_MATH_VECTOR<FLOAT, 2>_HH
