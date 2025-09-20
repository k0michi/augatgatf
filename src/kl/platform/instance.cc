#include "kl/platform/instance.hh"

#include <SDL3/SDL.h>
#include <string>

namespace kl::platform {
std::weak_ptr<Instance> Instance::sInstance;

std::expected<std::shared_ptr<Instance>, std::runtime_error>
Instance::create(const InstanceDescriptor &desc) noexcept {
  if (sInstance.lock()) {
    return std::unexpected(
        std::runtime_error("Instance::create: Instance already created"));
  }

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return std::unexpected(std::runtime_error(
        std::string("Instance::create: SDL_Init failed: ") + SDL_GetError()));
  }

  auto instance = std::shared_ptr<Instance>(new Instance());
  sInstance = instance;
  return instance;
}

Instance::~Instance() noexcept { SDL_Quit(); }

void Instance::pollEvents() noexcept {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      mShouldQuit = true;
    }
  }
}

bool Instance::shouldQuit() const noexcept { return mShouldQuit; }

std::expected<std::shared_ptr<Window>, std::runtime_error>
Instance::createWindow(const WindowDescriptor &descriptor) noexcept {
  return Window::create(descriptor);
}
} // namespace kl::platform