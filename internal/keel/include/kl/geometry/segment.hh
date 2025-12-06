#ifndef KL_GEOMETRY_SEGMENT_HH
#define KL_GEOMETRY_SEGMENT_HH

#include "kl/math/vector2.hh"

namespace kl::geometry {
template <typename T, std::size_t D> struct Segment;

template <typename T> struct Segment<T, 2> final {
  kl::math::Vector<T, 2> begin;
  kl::math::Vector<T, 2> end;
};

template <typename T> struct Segment<T, 3> final {
  kl::math::Vector<T, 3> begin;
  kl::math::Vector<T, 3> end;
};
} // namespace kl::geometry
#endif // KL_GEOMETRY_SEGMENT_HH