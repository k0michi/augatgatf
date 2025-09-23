#ifndef KL_GRAPHICS_DEVICE_HH
#define KL_GRAPHICS_DEVICE_HH

#include <cstdint>
#include <expected>
#include <memory>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "color_blend_state.hh"
#include "device_descriptor.hh"
#include "kl/platform/window.hh"
#include "opengl/gl_context.hh"
#include "program.hh"
#include "rasterization_state.hh"
#include "shader.hh"
#include "swapchain.hh"

namespace kl::graphics {
class Instance;

/**
 * @brief GL context manager.
 */
class Device : public std::enable_shared_from_this<Device> {
private:
  std::weak_ptr<Instance> mInstance;
  SDL_Window *mDefaultWindow = nullptr;
  /**
   * @details mContexts[0] is the default context. Every GL object is created in
   * the default context.
   */
  std::vector<std::shared_ptr<opengl::GLContext>> mContexts;
  std::unordered_map<std::shared_ptr<platform::Window>,
                     std::shared_ptr<opengl::GLContext>>
      mWindowContexts;

public:
  virtual ~Device() noexcept;

  Device(const Device &) = delete;
  Device(Device &&) noexcept = delete;
  Device &operator=(const Device &) = delete;
  Device &operator=(Device &&) noexcept = delete;

  std::expected<std::shared_ptr<RasterizationState>, std::runtime_error>
  createRasterizationState(
      const RasterizationStateDescriptor &descriptor) noexcept;
  std::expected<std::shared_ptr<Swapchain>, std::runtime_error>
  createSwapchain(const SwapchainDescriptor &descriptor) noexcept;
  std::expected<std::shared_ptr<Shader>, std::runtime_error>
  createShader(const ShaderDescriptor &descriptor) noexcept;
  std::expected<std::shared_ptr<Program>, std::runtime_error>
  createProgram(const ProgramDescriptor &descriptor) noexcept;
  std::expected<std::shared_ptr<ColorBlendState>, std::runtime_error>
  createColorBlendState(const ColorBlendStateDescriptor &descriptor) noexcept;

  /**
   * @brief Internal.
   */
  std::optional<std::shared_ptr<opengl::GLContext>>
  getContextForWindow(const std::shared_ptr<platform::Window> &window) noexcept;
  /**
   * @brief Internal.
   */
  std::expected<std::shared_ptr<opengl::GLContext>, std::runtime_error>
  getOrCreateContextForWindow(
      const std::shared_ptr<platform::Window> &window) noexcept;
  /**
   * @brief Internal.
   */
  bool
  hasContext(const std::shared_ptr<opengl::GLContext> &context) const noexcept;
  /**
   * @brief Internal.
   */
  bool hasContextForWindow(
      const std::shared_ptr<platform::Window> &window) const noexcept;
  /**
   * @brief Internal.
   */
  std::optional<std::shared_ptr<opengl::GLContext>>
  defaultContext() const noexcept;

  static std::expected<std::shared_ptr<Device>, std::runtime_error>
  create(std::shared_ptr<Instance> instance,
         const DeviceDescriptor &descriptor) noexcept;

protected:
  explicit Device() noexcept = default;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_DEVICE_HH
