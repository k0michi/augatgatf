#include "kl/platform/window.hh"

namespace kl::platform {
Window::~Window() {
  if (mWindow) {
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
  }
}

SDL_Window *Window::sdlWindow() const noexcept { return mWindow; }

std::expected<std::shared_ptr<Window>, std::runtime_error>
Window::create(const WindowDescriptor &descriptor) noexcept {
  auto sdlWindow = SDL_CreateWindow(descriptor.title.data(), descriptor.width,
                                    descriptor.height, 0);

  if (!sdlWindow) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  auto window = std::shared_ptr<Window>(new Window());
  window->mWindow = sdlWindow;
  return window;
}

std::expected<void, std::runtime_error> Window::show() noexcept {
  if (!SDL_ShowWindow(mWindow)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}

std::expected<void, std::runtime_error> Window::hide() noexcept {
  if (!SDL_HideWindow(mWindow)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}

std::expected<void, std::runtime_error> Window::focus() noexcept {
  if (!SDL_RaiseWindow(mWindow)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}
} // namespace kl::platform