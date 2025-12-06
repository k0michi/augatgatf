#ifndef KL_GEOMETRY_CAPSULE_HH
#define KL_GEOMETRY_CAPSULE_HH

#include "kl/geometry/segment.hh"

namespace kl::geometry {
template <typename T, std::size_t D> struct Capsule final {
  Segment<T, D> segment;
  T radius;
};
} // namespace kl::geometry
#endif // KL_GEOMETRY_CAPSULE_HH