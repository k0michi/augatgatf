#ifndef KL_GRAPHICS_SURFACE_HH
#define KL_GRAPHICS_SURFACE_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "surface_descriptor.hh"

namespace kl::graphics {
/**
 * @brief A class holding a window.
 */
class Surface {
private:
  std::shared_ptr<platform::Window> mWindow;

public:
  virtual ~Surface() noexcept = default;

  Surface(const Surface &) = delete;
  Surface(Surface &&) noexcept = default;
  Surface &operator=(const Surface &) = delete;
  Surface &operator=(Surface &&) noexcept = default;

  static std::expected<std::shared_ptr<Surface>, std::runtime_error>
  create(const SurfaceDescriptor &descriptor) noexcept;
  // TODO: Add framebuffer acquisition

protected:
  explicit Surface() noexcept = default;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_SURFACE_HH
