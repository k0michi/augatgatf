#include "kl/graphics/device.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<Device>, std::runtime_error>
Device::create(const DeviceDescriptor &descriptor) noexcept {
  auto device = std::shared_ptr<Device>(new Device());
  return device;
}
} // namespace kl::graphics
