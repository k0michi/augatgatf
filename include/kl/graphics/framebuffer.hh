#ifndef KL_GRAPHICS_FRAMEBUFFER_HH
#define KL_GRAPHICS_FRAMEBUFFER_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "device_child.hh"
#include "framebuffer_descriptor.hh"

namespace kl::platform {
class Window;
}

namespace kl::graphics {
/**
 * @brief Framebuffer object. Represents a created framebuffer, or the default
 * framebuffer for a widnow.
 */
class Framebuffer : public DeviceChild {
private:
  FramebufferDescriptor mDescriptor;
  /**
   * @brief Not null if this object represents a window's default framebuffer.
   */
  std::shared_ptr<platform::Window> mAssociatedWindow;
  /**
   * @brief OpenGL framebuffer object. 0 if this object represents a window's
   * default framebuffer.
   */
  GLuint mFramebuffer = 0;

public:
  virtual ~Framebuffer() noexcept = default;

  Framebuffer(const Framebuffer &) = delete;
  Framebuffer(Framebuffer &&) noexcept = delete;
  Framebuffer &operator=(const Framebuffer &) = delete;
  Framebuffer &operator=(Framebuffer &&) noexcept = delete;

  inline const FramebufferDescriptor &descriptor() const noexcept {
    return mDescriptor;
  }

  inline bool isDefaultFramebuffer() const noexcept {
    return mAssociatedWindow != nullptr;
  }

  inline GLuint glFramebuffer() const noexcept { return mFramebuffer; }

  inline std::shared_ptr<platform::Window> associatedWindow() const noexcept {
    return mAssociatedWindow;
  }

  static std::expected<std::shared_ptr<Framebuffer>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const FramebufferDescriptor &descriptor) noexcept;
  static std::expected<std::shared_ptr<Framebuffer>, std::runtime_error>
  createDefault(std::shared_ptr<Device> device,
                std::shared_ptr<platform::Window> window) noexcept;

private:
  explicit Framebuffer(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_FRAMEBUFFER_HH
