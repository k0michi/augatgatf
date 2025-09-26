#ifndef KL_PLATFORM_WINDOW_DESCRIPTOR_HH
#define KL_PLATFORM_WINDOW_DESCRIPTOR_HH

#include <cstdint>
#include <string>

namespace kl::platform {
struct WindowDescriptor final {
  std::string title;
  int32_t width;
  int32_t height;
};
} // namespace kl::platform
#endif // KL_PLATFORM_WINDOW_DESCRIPTOR_HH