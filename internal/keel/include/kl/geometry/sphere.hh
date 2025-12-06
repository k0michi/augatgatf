#ifndef KL_GEOMETRY_CIRCLE_HH
#define KL_GEOMETRY_CIRCLE_HH

#include "kl/math/vector2.hh"

namespace kl::geometry {
template <typename T, std::size_t D> struct Sphere;

template <typename T> struct Sphere<T, 2> final {
  kl::math::Vector<T, 2> center;
  T radius;
};

template <typename T> using Circle = Sphere<T, 2>;

template <typename T> struct Sphere<T, 3> final {
  kl::math::Vector<T, 3> center;
  T radius;
};
} // namespace kl::geometry
#endif // KL_GEOMETRY_SPHERE_HH