#include "kl/graphics/device_child.hh"

namespace kl::graphics {
DeviceChild::DeviceChild(std::shared_ptr<Device> device) noexcept
    : mDevice(device) {}
} // namespace kl::graphics
