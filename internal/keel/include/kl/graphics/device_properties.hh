#ifndef KL_GRAPHICS_DEVICE_PROPERTIES_HH
#define KL_GRAPHICS_DEVICE_PROPERTIES_HH

#include <string>

namespace kl::graphics {
struct DeviceLimits {
  // TODO
};

struct DeviceProperties final {
  std::string renderer;
  std::string vendor;
  std::string version;
  DeviceLimits limits;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_DEVICE_PROPERTIES_HH
