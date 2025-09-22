#ifndef KL_MATH_VECTOR2_HH
#define KL_MATH_VECTOR2_HH

#include <cmath>
#include <stdexcept>

namespace kl::math {
template <typename T, std::size_t N> struct Vector;

constexpr Vector<float, 2> operator+(Vector<float, 2> lhs,
                                     const Vector<float, 2> &rhs) noexcept;
constexpr Vector<float, 2> operator-(Vector<float, 2> lhs,
                                     const Vector<float, 2> &rhs) noexcept;
constexpr Vector<float, 2> operator*(float lhs, Vector<float, 2> rhs) noexcept;

template <> struct Vector<float, 2> final {
  float x;
  float y;

  static constexpr Vector<float, 2> zero() noexcept {
    return Vector<float, 2>{0.0f, 0.0f};
  }

  static constexpr Vector<float, 2> one() noexcept {
    return Vector<float, 2>{1.0f, 1.0f};
  }

  static constexpr Vector<float, 2> unitX() noexcept {
    return Vector<float, 2>{1.0f, 0.0f};
  }

  static constexpr Vector<float, 2> unitY() noexcept {
    return Vector<float, 2>{0.0f, 1.0f};
  }

  constexpr float lengthSquared() const noexcept { return x * x + y * y; }

  constexpr float length() const noexcept { return std::sqrt(lengthSquared()); }

  constexpr void normalize() noexcept {
    float len = length();

    if (len == 0.0f) {
      (*this) = zero();
      return;
    }

    (*this) /= len;
  }

  static constexpr Vector<float, 2>
  normalize(const Vector<float, 2> &v) noexcept {
    Vector<float, 2> result = v;
    result.normalize();
    return result;
  }

  static constexpr float dot(const Vector<float, 2> &a,
                             const Vector<float, 2> &b) noexcept {
    return a.x * b.x + a.y * b.y;
  }

  static constexpr float cross(const Vector<float, 2> &a,
                               const Vector<float, 2> &b) noexcept {
    return a.x * b.y - a.y * b.x;
  }

  static constexpr float distance(const Vector<float, 2> &a,
                                  const Vector<float, 2> &b) noexcept {
    return (a - b).length();
  }

  static constexpr float distanceSquared(const Vector<float, 2> &a,
                                         const Vector<float, 2> &b) noexcept {
    return (a - b).lengthSquared();
  }

  static constexpr Vector<float, 2>
  lerp(const Vector<float, 2> &a, const Vector<float, 2> &b, float t) noexcept {
    return a + t * (b - a);
  }

  constexpr float &operator[](std::size_t index) {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  constexpr Vector<float, 2> &
  operator+=(const Vector<float, 2> &other) noexcept {
    x += other.x;
    y += other.y;
    return *this;
  }

  constexpr Vector<float, 2> &
  operator-=(const Vector<float, 2> &other) noexcept {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  constexpr Vector<float, 2> &operator*=(float other) noexcept {
    x *= other;
    y *= other;
    return *this;
  }

  constexpr Vector<float, 2> &
  operator*=(const Vector<float, 2> &other) noexcept {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  constexpr Vector<float, 2> &operator/=(float other) noexcept {
    x /= other;
    y /= other;
    return *this;
  }

  constexpr Vector<float, 2> &
  operator/=(const Vector<float, 2> &other) noexcept {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  constexpr Vector<float, 2> operator-() const noexcept {
    return Vector<float, 2>{-x, -y};
  }
};

constexpr Vector<float, 2> operator+(Vector<float, 2> lhs,
                                     const Vector<float, 2> &rhs) noexcept {
  lhs += rhs;
  return lhs;
}

constexpr Vector<float, 2> operator-(Vector<float, 2> lhs,
                                     const Vector<float, 2> &rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

constexpr Vector<float, 2> operator*(Vector<float, 2> lhs, float rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

constexpr Vector<float, 2> operator*(float lhs, Vector<float, 2> rhs) noexcept {
  rhs *= lhs;
  return rhs;
}

constexpr Vector<float, 2> operator*(Vector<float, 2> lhs,
                                     const Vector<float, 2> &rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

constexpr Vector<float, 2> operator/(Vector<float, 2> lhs, float rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

constexpr Vector<float, 2> operator/(Vector<float, 2> lhs,
                                     const Vector<float, 2> &rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

constexpr Vector<float, 2> operator/(float lhs,
                                     const Vector<float, 2> &rhs) noexcept {
  Vector<float, 2> result{lhs, lhs};
  result /= rhs;
  return result;
}

constexpr bool operator==(const Vector<float, 2> &lhs,
                          const Vector<float, 2> &rhs) noexcept {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

constexpr bool operator!=(const Vector<float, 2> &lhs,
                          const Vector<float, 2> &rhs) noexcept {
  return !(lhs == rhs);
}

using Vector2 = Vector<float, 2>;
} // namespace kl::math
#endif // KL_MATH_VECTOR<FLOAT, 2>_HH
