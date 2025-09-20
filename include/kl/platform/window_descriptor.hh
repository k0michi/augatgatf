#ifndef KL_PLATFORM_WINDOW_DESCRIPTOR_HH
#define KL_PLATFORM_WINDOW_DESCRIPTOR_HH

#include <cstdint>
#include <string_view>

namespace kl::platform {
struct WindowDescriptor {
  std::string_view title;
  int32_t width;
  int32_t height;
};
} // namespace kl::platform
#endif // KL_PLATFORM_WINDOW_DESCRIPTOR_HH