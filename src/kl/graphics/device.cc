#include "kl/graphics/device.hh"

namespace kl::graphics {
Device::~Device() noexcept {
  if (mDefaultWindow) {
    SDL_DestroyWindow(mDefaultWindow);
    mDefaultWindow = nullptr;
  }
}

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
  mContexts.push_back(glContext);
  mSurfaceContexts[surface] = glContext;
  return glContext;
}

bool Device::hasContext(
    const std::shared_ptr<opengl::GLContext> &context) const noexcept {
  return std::find(mContexts.begin(), mContexts.end(), context) !=
         mContexts.end();
}

bool Device::hasContextForSurface(
    const std::shared_ptr<Surface> &surface) const noexcept {
  return mSurfaceContexts.find(surface) != mSurfaceContexts.end();
}

std::optional<std::shared_ptr<opengl::GLContext>>
Device::defaultContext() const noexcept {
  if (mContexts.empty()) {
    return std::nullopt;
  }

  return *mContexts.begin();
}

std::expected<std::shared_ptr<Shader>, std::runtime_error>
Device::createShader(const ShaderDescriptor &descriptor) noexcept {
  return Shader::create(shared_from_this(), descriptor);
}

std::expected<std::shared_ptr<Program>, std::runtime_error>
Device::createProgram(const ProgramDescriptor &descriptor) noexcept {
  return Program::create(shared_from_this(), descriptor);
}

std::expected<std::shared_ptr<Device>, std::runtime_error>
Device::create(std::shared_ptr<Instance> instance,
               const DeviceDescriptor &descriptor) noexcept {
  auto device = std::shared_ptr<Device>(new Device());
  device->mInstance = instance;

#ifndef __EMSCRIPTEN__
  device->mDefaultWindow =
      SDL_CreateWindow("Default Window", 1, 1, SDL_WINDOW_OPENGL);

  if (!device->mDefaultWindow) {
    return std::unexpected(
        std::runtime_error("Failed to create default SDL window"));
  }

  auto defaultContextResult = opengl::GLContext::create({
      .window = device->mDefaultWindow,
      .shareContext = nullptr,
      .profile = SDL_GL_CONTEXT_PROFILE_CORE,
      .majorVersion = 3,
      .minorVersion = 3,
  });

  if (!defaultContextResult) {
    return std::unexpected(defaultContextResult.error());
  }

  device->mContexts.emplace_back(defaultContextResult.value());
#endif

  return device;
}
} // namespace kl::graphics
