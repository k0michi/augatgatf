#ifndef KL_GEOMETRY_CIRCLE_HH
#define KL_GEOMETRY_CIRCLE_HH

#include <numbers>

#include "kl/geometry/containment.hh"
#include "kl/math/vector2.hh"
#include "kl/math/vector3.hh"

namespace kl::geometry {
// REF:
// https://github.com/MonoGame/MonoGame/blob/develop/MonoGame.Framework/BoundingSphere.cs
template <typename T, std::size_t D> struct Sphere;

template <typename T> struct Sphere<T, 2> final {
  kl::math::Vector<T, 2> center;
  T radius;

  T area() const noexcept {
    return static_cast<T>(std::numbers::pi) * radius * radius;
  }

  T circumference() const noexcept {
    return static_cast<T>(2) * static_cast<T>(std::numbers::pi) * radius;
  }

  T diameter() const noexcept { return static_cast<T>(2) * radius; }

  T volume() const noexcept { return area(); }

  T surfaceArea() const noexcept { return circumference(); }

  /**
   * @brief Calculates the signed distance between this circle and another
   * circle.
   */
  T distance(const Sphere<T, 2> &other) const noexcept {
    T centerDistance = (other.center - center).length();
    T radiusSum = radius + other.radius;
    return centerDistance - radiusSum;
  }

  /**
   * @brief Calculates the signed distance between this circle and a point.
   */
  T distance(const kl::math::Vector<T, 2> &point) const noexcept {
    T centerToPointDistance = (point - center).length();
    return centerToPointDistance - radius;
  }

  /**
   * @brief Checks if this circle intersects with another circle.
   */
  bool intersects(const Sphere<T, 2> &other) const noexcept {
    return distance(other) <= static_cast<T>(0);
  }

  /**
   * @brief Checks if this circle intersects with a point.
   */
  bool intersects(const kl::math::Vector<T, 2> &point) const noexcept {
    T centerToPointDistanceSq = (point - center).lengthSquared();
    T radiusSq = radius * radius;
    return centerToPointDistanceSq <= radiusSq;
  }

  /**
   * @brief Determines the containment relationship between this circle and a
   * point.
   */
  Containment contains(const kl::math::Vector<T, 2> &point) const noexcept {
    T centerToPointDistanceSq = (point - center).lengthSquared();
    T radiusSq = radius * radius;

    if (centerToPointDistanceSq < radiusSq) {
      return Containment::eContains;
    } else if (centerToPointDistanceSq == radiusSq) {
      return Containment::eIntersects;
    } else {
      return Containment::eDisjoint;
    }
  }

  /**
   * @brief Determines the containment relationship between this circle and
   * another circle.
   */
  Containment contains(const Sphere<T, 2> &other) const noexcept {
    T centerDistanceSq = (other.center - center).lengthSquared();
    T radiusDiff = radius - other.radius;
    T radiusDiffSq = radiusDiff * radiusDiff;

    if (centerDistanceSq < radiusDiffSq) {
      return Containment::eContains;
    } else if (centerDistanceSq == radiusDiffSq) {
      return Containment::eIntersects;
    } else {
      return Containment::eDisjoint;
    }
  }
};

template <typename T> using Circle = Sphere<T, 2>;

template <typename T> struct Sphere<T, 3> final {
  kl::math::Vector<T, 3> center;
  T radius;

  T volume() const noexcept {
    return (static_cast<T>(4) / static_cast<T>(3)) *
           static_cast<T>(std::numbers::pi) * radius * radius * radius;
  }

  T surfaceArea() const noexcept {
    return static_cast<T>(4) * static_cast<T>(std::numbers::pi) * radius *
           radius;
  }

  T diameter() const noexcept { return static_cast<T>(2) * radius; }

  /**
   * @brief Calculates the signed distance between this sphere and another
   * sphere.
   */
  T distance(const Sphere<T, 3> &other) const noexcept {
    T centerDistance = (other.center - center).length();
    T radiusSum = radius + other.radius;
    return centerDistance - radiusSum;
  }

  /**
   * @brief Calculates the signed distance between this sphere and a point.
   */
  T distance(const kl::math::Vector<T, 3> &point) const noexcept {
    T centerToPointDistance = (point - center).length();
    return centerToPointDistance - radius;
  }

  /**
   * @brief Checks if this sphere intersects with another sphere.
   */
  bool intersects(const Sphere<T, 3> &other) const noexcept {
    T distSq = (other.center - center).lengthSquared();
    T radiusSum = radius + other.radius;
    return distSq <= radiusSum * radiusSum;
  }

  /**
   * @brief Determines the containment relationship between this sphere and a
   * point.
   */
  Containment contains(const kl::math::Vector<T, 3> &point) const noexcept {
    T centerToPointDistanceSq = (point - center).lengthSquared();
    T radiusSq = radius * radius;

    if (centerToPointDistanceSq < radiusSq) {
      return Containment::eContains;
    } else if (centerToPointDistanceSq == radiusSq) {
      return Containment::eIntersects;
    } else {
      return Containment::eDisjoint;
    }
  }

  /**
   * @brief Determines the containment relationship between this sphere and
   * another sphere.
   */
  Containment contains(const Sphere<T, 3> &other) const noexcept {
    T centerDistanceSq = (other.center - center).lengthSquared();
    T radiusDiff = radius - other.radius;
    T radiusDiffSq = radiusDiff * radiusDiff;

    if (centerDistanceSq < radiusDiffSq) {
      return Containment::eContains;
    } else if (centerDistanceSq == radiusDiffSq) {
      return Containment::eIntersects;
    } else {
      return Containment::eDisjoint;
    }
  }
};
} // namespace kl::geometry
#endif // KL_GEOMETRY_SPHERE_HH