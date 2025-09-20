#pragma once

#include <cstdint>
#include <string_view>

namespace kl::platform {
struct WindowDescriptor {
  std::string_view title;
  int32_t width;
  int32_t height;
};
} // namespace kl::platform