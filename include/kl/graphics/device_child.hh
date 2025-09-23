#ifndef KL_GRAPHICS_DEVICE_CHILD_HH
#define KL_GRAPHICS_DEVICE_CHILD_HH

#include <memory>

namespace kl::graphics {
class Device;

class DeviceChild {
private:
  std::weak_ptr<Device> mDevice;

public:
  explicit DeviceChild(std::shared_ptr<Device> device) noexcept;
  virtual ~DeviceChild() noexcept = default;

  DeviceChild(const DeviceChild &) = delete;
  DeviceChild(DeviceChild &&) noexcept = delete;
  DeviceChild &operator=(const DeviceChild &) = delete;
  DeviceChild &operator=(DeviceChild &&) noexcept = delete;

  std::weak_ptr<Device> device() const noexcept { return mDevice; }
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_DEVICE_CHILD_HH
