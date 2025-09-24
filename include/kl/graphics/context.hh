#ifndef KL_GRAPHICS_CONTEXT_HH
#define KL_GRAPHICS_CONTEXT_HH

#include <expected>
#include <memory>
#include <optional>
#include <stdexcept>

#include "color_blend_state.hh"
#include "context_descriptor.hh"
#include "device_child.hh"
#include "framebuffer.hh"
#include "kl/common/rectangle2.hh"
#include "opengl/gl_context.hh"
#include "program.hh"
#include "rasterization_state.hh"
#include "scissor_rect.hh"
#include "viewport.hh"

namespace kl::graphics {
class Device;

struct ContextState final {
  std::shared_ptr<Framebuffer> framebuffer;
  std::optional<Viewport> viewport;
  std::optional<ScissorRect> scissorRect;
  std::shared_ptr<Program> program;
  std::shared_ptr<ColorBlendState> colorBlendState;
  std::shared_ptr<RasterizationState> rasterizationState;
};

/**
 * @brief Rendering context. Context is where users dispatch rendering commands.
 */
// TODO: Proper error handling.
class Context : public DeviceChild {
private:
  ContextDescriptor mDescriptor;
  ContextState mState;
  bool mFramebufferDirty = true;
  bool mViewportDirty = true;
  bool mScissorRectDirty = true;
  bool mProgramDirty = true;
  bool mColorBlendStateDirty = true;
  bool mRasterizationStateDirty = true;

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
  void setViewport(const std::optional<Viewport> &viewport) noexcept;
  const std::optional<Viewport> &getViewport() const noexcept;
  void setScissorRect(const std::optional<ScissorRect> &rect) noexcept;
  const std::optional<ScissorRect> &getScissorRect() const noexcept;
  void setProgram(std::shared_ptr<Program> program) noexcept;
  const std::shared_ptr<Program> &getProgram() const noexcept;
  void
  setColorBlendState(std::shared_ptr<ColorBlendState> colorBlendState) noexcept;
  const std::shared_ptr<ColorBlendState> &getColorBlendState() const noexcept;
  void setRasterizationState(
      std::shared_ptr<RasterizationState> rasterizationState) noexcept;
  const std::shared_ptr<RasterizationState> &
  getRasterizationState() const noexcept;

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
