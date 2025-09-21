#ifndef KL_GRAPHICS_DEVICE_HH
#define KL_GRAPHICS_DEVICE_HH

#include <cstdint>
#include <expected>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "device_descriptor.hh"
#include "opengl/gl_context.hh"
#include "surface.hh"

namespace kl::graphics {
/**
 * @brief GL context manager.
 */
class Device {
private:
  std::unordered_map<uint32_t, std::shared_ptr<opengl::GLContext>> mContexts;
  std::unordered_map<std::shared_ptr<Surface>,
                     std::shared_ptr<opengl::GLContext>>
      mSurfaceContexts;

public:
  virtual ~Device() noexcept = default;

  Device(const Device &) = delete;
  Device(Device &&) noexcept = delete;
  Device &operator=(const Device &) = delete;
  Device &operator=(Device &&) noexcept = delete;

  static std::expected<std::shared_ptr<Device>, std::runtime_error>
  create(const DeviceDescriptor &descriptor) noexcept;

private:
  explicit Device() noexcept = default;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_DEVICE_HH
