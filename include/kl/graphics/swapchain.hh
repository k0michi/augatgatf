#ifndef KL_GRAPHICS_SWAPCHAIN_HH
#define KL_GRAPHICS_SWAPCHAIN_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "device_child.hh"
#include "framebuffer.hh"
#include "swapchain_descriptor.hh"

namespace kl::graphics {
class Device;

class Swapchain : public DeviceChild {
  SwapchainDescriptor mDescriptor;
  std::shared_ptr<Framebuffer> mFramebuffer;

public:
  virtual ~Swapchain() noexcept = default;

  Swapchain(const Swapchain &) = delete;
  Swapchain(Swapchain &&) noexcept = delete;
  Swapchain &operator=(const Swapchain &) = delete;
  Swapchain &operator=(Swapchain &&) noexcept = delete;

  const SwapchainDescriptor &descriptor() const noexcept { return mDescriptor; }

  std::shared_ptr<Framebuffer> framebuffer() const noexcept {
    return mFramebuffer;
  }

  static std::expected<std::shared_ptr<Swapchain>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const SwapchainDescriptor &descriptor) noexcept;

private:
  explicit Swapchain(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_SWAPCHAIN_HH
