#ifndef KL_GEOMETRY_TRIANGLE_HH
#define KL_GEOMETRY_TRIANGLE_HH

#include "kl/math/vector2.hh"
#include "kl/math/vector3.hh"

namespace kl::geometry {
template <typename T, std::size_t D> struct Triangle;

template <typename T> struct Triangle<T, 2> final {
  kl::math::Vector<T, 2> v0;
  kl::math::Vector<T, 2> v1;
  kl::math::Vector<T, 2> v2;
};

template <typename T> struct Triangle<T, 3> final {
  kl::math::Vector<T, 3> v0;
  kl::math::Vector<T, 3> v1;
  kl::math::Vector<T, 3> v2;
};
} // namespace kl::geometry

#endif