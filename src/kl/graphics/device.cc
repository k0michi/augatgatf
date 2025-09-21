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

std::expected<std::shared_ptr<RasterizationState>, std::runtime_error>
Device::createRasterizationState(
    const RasterizationStateDescriptor &descriptor) const noexcept {
  return RasterizationState::create(descriptor);
}

std::expected<std::shared_ptr<opengl::GLContext>, std::runtime_error>
Device::getOrCreateContextForSurface(
    const std::shared_ptr<Surface> &surface) noexcept {
  auto existingContext = getContextForSurface(surface);
  if (existingContext) {
    return existingContext.value();
  }

#ifndef __EMSCRIPTEN__
  auto glContextResult = opengl::GLContext::create({
      .window = surface->window()->sdlWindow(),
      .shareContext = nullptr,
      .profile = SDL_GL_CONTEXT_PROFILE_CORE,
      .majorVersion = 3,
      .minorVersion = 3,
  });
#else
  auto glContextResult = opengl::GLContext::create({
      .window = surface->window()->sdlWindow(),
      .shareContext = nullptr,
      .profile = SDL_GL_CONTEXT_PROFILE_ES,
      .majorVersion = 3,
      .minorVersion = 0,
  });
#endif
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
Device::create(std::shared_ptr<Instance> instance,
               const DeviceDescriptor &descriptor) noexcept {
  auto device = std::shared_ptr<Device>(new Device());
  device->mInstance = instance;
  return device;
}
} // namespace kl::graphics
