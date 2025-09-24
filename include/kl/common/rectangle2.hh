#ifndef KL_COMMON_RECTANGLE2_HH
#define KL_COMMON_RECTANGLE2_HH

#include <type_traits>

#include "kl/common/extent2.hh"
#include "kl/common/offset2.hh"

namespace kl::common {
template <typename T, typename U = std::make_unsigned_t<T>>
struct Rectangle2 final {
  Offset2<T> offset;
  Extent2<U> extent;
};
} // namespace kl::common
#endif // KL_COMMON_RECTANGLE2_HH
