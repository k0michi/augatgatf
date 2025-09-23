#ifndef KL_PLATFORM_WINDOW_HH
#define KL_PLATFORM_WINDOW_HH

#include <cstdint>
#include <expected>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include <SDL3/SDL.h>

#include "kl/common/extent2.hh"
#include "kl/common/point2.hh"
#include "kl/graphics/opengl/surface_config.hh"
#include "window_descriptor.hh"

namespace kl::platform {
class Window final {
private:
  SDL_Window *mWindow;
  kl::graphics::opengl::SurfaceConfig mGLSurfaceConfig;

private:
  Window() = default;

public:
  ~Window();

  SDL_Window *sdlWindow() const noexcept;

  std::expected<void, std::runtime_error> show() noexcept;
  std::expected<void, std::runtime_error> hide() noexcept;
  std::expected<void, std::runtime_error> focus() noexcept;
  std::expected<void, std::runtime_error> minimize() noexcept;
  std::expected<void, std::runtime_error> maximize() noexcept;
  std::expected<void, std::runtime_error> restore() noexcept;

  std::expected<kl::common::Extent2<int32_t>, std::runtime_error>
  size() const noexcept;
  std::expected<void, std::runtime_error>
  setSize(const kl::common::Extent2<int32_t> &size) noexcept;
  std::expected<kl::common::Extent2<int32_t>, std::runtime_error>
  sizeInPixels() const noexcept;
  std::expected<kl::common::Extent2<int32_t>, std::runtime_error>
  maximumSize() const noexcept;
  std::expected<void, std::runtime_error>
  setMaximumSize(const kl::common::Extent2<int32_t> &size) noexcept;
  std::expected<kl::common::Extent2<int32_t>, std::runtime_error>
  minimumSize() const noexcept;
  std::expected<void, std::runtime_error>
  setMinimumSize(const kl::common::Extent2<int32_t> &size) noexcept;

  std::expected<kl::common::Point2<int32_t>, std::runtime_error>
  position() const noexcept;
  std::expected<void, std::runtime_error>
  setPosition(const kl::common::Point2<int32_t> &position) noexcept;

  std::string title() const noexcept;
  std::expected<void, std::runtime_error>
  setTitle(std::string_view title) noexcept;

  bool isResizable() const noexcept;
  std::expected<void, std::runtime_error> setResizable(bool resizable) noexcept;

  /**
   * @brief Internal.
   */
  void setSDLGLAttributes() const noexcept;
  /**
   * @brief Internal.
   */
  const kl::graphics::opengl::SurfaceConfig &glSurfaceConfig() const noexcept;
  /**
   * @brief Internal.
   */
  void setGLSurfaceConfig(const kl::graphics::opengl::SurfaceConfig &config);
  /**
   * @brief Internal.
   */
  std::expected<void, std::runtime_error> recreate() noexcept;

  static std::expected<std::shared_ptr<Window>, std::runtime_error>
  create(const WindowDescriptor &descriptor) noexcept;
};
} // namespace kl::platform
#endif // KL_PLATFORM_WINDOW_HH