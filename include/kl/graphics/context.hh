#ifndef KL_GRAPHICS_CONTEXT_HH
#define KL_GRAPHICS_CONTEXT_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "context_descriptor.hh"
#include "device_child.hh"
#include "framebuffer.hh"
#include "kl/graphics/opengl/gl_context.hh"

namespace kl::graphics {
class Device;

struct ContextState final {
  std::shared_ptr<Framebuffer> framebuffer;
};

class Context : public DeviceChild {
private:
  ContextDescriptor mDescriptor;
  ContextState mState;

public:
  virtual ~Context() noexcept = default;

  Context(const Context &) = delete;
  Context(Context &&) noexcept = delete;
  Context &operator=(const Context &) = delete;
  Context &operator=(Context &&) noexcept = delete;

  inline const ContextDescriptor &descriptor() const noexcept {
    return mDescriptor;
  }

  void setFramebuffer(std::shared_ptr<Framebuffer> framebuffer) noexcept;
  void clearColor(std::tuple<float, float, float, float> color) noexcept;
  void clearDepthStencil(float depth, int32_t stencil) noexcept;

  static std::expected<std::shared_ptr<Context>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const ContextDescriptor &descriptor) noexcept;

private:
  explicit Context(std::shared_ptr<Device> device) noexcept;

  void applyState() noexcept;

  std::optional<std::shared_ptr<opengl::GLContext>>
  currentGLContext() const noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_CONTEXT_HH
