#ifndef KL_GEOMETRY_LINE_HH
#define KL_GEOMETRY_LINE_HH

#include "kl/math/vector2.hh"

namespace kl::geometry {
template <typename T, std::size_t D> struct Line;

template <typename T> struct Line<T, 2> final {
  kl::math::Vector<T, 2> position;
  kl::math::Vector<T, 2> direction;
};

template <typename T> struct Line<T, 3> final {
  kl::math::Vector<T, 3> position;
  kl::math::Vector<T, 3> direction;
};

} // namespace kl::geometry

#endif