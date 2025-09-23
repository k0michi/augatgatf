#include "kl/graphics/surface.hh"

namespace kl::graphics {
const SurfaceDescriptor &Surface::descriptor() const noexcept {
  return mDescriptor;
}

std::shared_ptr<platform::Window> Surface::window() const noexcept {
  return mWindow;
}

std::expected<std::shared_ptr<Surface>, std::runtime_error>
Surface::create(std::shared_ptr<Instance> instance,
                const SurfaceDescriptor &descriptor) noexcept {
  if (!descriptor.window) {
    return std::unexpected(
        std::runtime_error("Surface::create: window is null"));
  }

  auto surface = std::shared_ptr<Surface>(new Surface());
  surface->mWindow = descriptor.window;
  surface->mDescriptor = descriptor;
  return surface;
}
} // namespace kl::graphics
