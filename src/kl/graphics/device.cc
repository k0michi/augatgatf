#include "kl/graphics/device.hh"

namespace kl::graphics {
std::optional<std::shared_ptr<opengl::GLContext>>
Device::getContextForSurface(const std::shared_ptr<Surface> &surface) noexcept {
  auto it = mSurfaceContexts.find(surface);
  if (it != mSurfaceContexts.end()) {
    return it->second;
  }
  return std::nullopt;
}

std::expected<std::shared_ptr<opengl::GLContext>, std::runtime_error>
Device::getOrCreateContextForSurface(
    const std::shared_ptr<Surface> &surface) noexcept {
  auto existingContext = getContextForSurface(surface);
  if (existingContext) {
    return existingContext.value();
  }

  auto glContextResult =
      opengl::GLContext::create(surface->window()->sdlWindow());
  if (!glContextResult) {
    return std::unexpected(glContextResult.error());
  }

  auto glContext = glContextResult.value();
  mContexts.insert(glContext);
  mSurfaceContexts[surface] = glContext;
  return glContext;
}

bool Device::hasContext(
    const std::shared_ptr<opengl::GLContext> &context) const noexcept {
  return mContexts.find(context) != mContexts.end();
}

bool Device::hasContextForSurface(
    const std::shared_ptr<Surface> &surface) const noexcept {
  return mSurfaceContexts.find(surface) != mSurfaceContexts.end();
}

std::expected<std::shared_ptr<Device>, std::runtime_error>
Device::create(const DeviceDescriptor &descriptor) noexcept {
  auto device = std::shared_ptr<Device>(new Device());
  return device;
}
} // namespace kl::graphics
