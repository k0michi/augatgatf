#include "kl/graphics/surface.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<Surface>, std::runtime_error>
Surface::create(const SurfaceDescriptor &descriptor) noexcept {
  if (!descriptor.window) {
    return std::unexpected(
        std::runtime_error("Surface::create: window is null"));
  }

  auto surface = std::shared_ptr<Surface>(new Surface());
  surface->mWindow = descriptor.window;
  return surface;
}
} // namespace kl::graphics
