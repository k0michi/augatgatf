#ifndef KL_COMMON_BOX_HH
#define KL_COMMON_BOX_HH

#include <type_traits>

#include "kl/common/extent3.hh"
#include "kl/common/offset3.hh"

namespace kl::common {
template <typename T, typename U = std::make_unsigned_t<T>> struct Box final {
  Offset3<T> offset;
  Extent3<U> extent;
};
} // namespace kl::common
#endif // KL_COMMON_BOX_HH