#ifndef KL_COMMON_EXTENT3_HH
#define KL_COMMON_EXTENT3_HH

namespace kl::common {
template <typename T> struct Extent3 final {
  T width;
  T height;
  T depth;
};
} // namespace kl::common
#endif // KL_COMMON_EXTENT3_HH