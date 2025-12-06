#ifndef KL_GEOMETRY_RAY_HH
#define KL_GEOMETRY_RAY_HH

#include "kl/math/vector2.hh"

namespace kl::geometry {
template <typename T, std::size_t D> struct Ray;

template <typename T> struct Ray<T, 2> final {
  kl::math::Vector<T, 2> position;
  kl::math::Vector<T, 2> direction;
};

template <typename T> struct Ray<T, 3> final {
  kl::math::Vector<T, 3> position;
  kl::math::Vector<T, 3> direction;
};
} // namespace kl::geometry
#endif // KL_GEOMETRY_RAY_HH