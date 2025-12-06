#ifndef KL_GEOMETRY_PLANE_HH
#define KL_GEOMETRY_PLANE_HH

#include "kl/math/vector3.hh"

namespace kl::geometry {
template <typename T, std::size_t D> struct Plane;

template <typename T> struct Plane<T, 2> final {
  kl::math::Vector<T, 2> normal;
  T d;
};

template <typename T> struct Plane<T, 3> final {
  kl::math::Vector<T, 3> normal;
  T d;
};
} // namespace kl::geometry
#endif // KL_GEOMETRY_PLANE_HH