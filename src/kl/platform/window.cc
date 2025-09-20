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

std::expected<void, std::runtime_error> Window::minimize() noexcept {
  if (!SDL_MinimizeWindow(mWindow)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}

std::expected<void, std::runtime_error> Window::maximize() noexcept {
  if (!SDL_MaximizeWindow(mWindow)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}

std::expected<void, std::runtime_error> Window::restore() noexcept {
  if (!SDL_RestoreWindow(mWindow)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}

std::expected<kl::common::Extent2<int32_t>, std::runtime_error>
Window::size() const noexcept {
  int w, h;
  SDL_GetWindowSize(mWindow, &w, &h);
  return kl::common::Extent2<int32_t>{w, h};
}

std::expected<void, std::runtime_error>
Window::setSize(const kl::common::Extent2<int32_t> &size) noexcept {
  if (!SDL_SetWindowSize(mWindow, size.width, size.height)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}

std::expected<kl::common::Extent2<int32_t>, std::runtime_error>
Window::sizeInPixels() const noexcept {
  int w, h;

  if (!SDL_GetWindowSizeInPixels(mWindow, &w, &h)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return kl::common::Extent2<int32_t>{w, h};
}

std::expected<kl::common::Extent2<int32_t>, std::runtime_error>
Window::maximumSize() const noexcept {
  int w, h;

  if (!SDL_GetWindowMaximumSize(mWindow, &w, &h)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return kl::common::Extent2<int32_t>{w, h};
}

std::expected<void, std::runtime_error>
Window::setMaximumSize(const kl::common::Extent2<int32_t> &size) noexcept {
  if (!SDL_SetWindowMaximumSize(mWindow, size.width, size.height)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}

std::expected<kl::common::Extent2<int32_t>, std::runtime_error>
Window::minimumSize() const noexcept {
  int w, h;

  if (!SDL_GetWindowMinimumSize(mWindow, &w, &h)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return kl::common::Extent2<int32_t>{w, h};
}

std::expected<void, std::runtime_error>
Window::setMinimumSize(const kl::common::Extent2<int32_t> &size) noexcept {
  if (!SDL_SetWindowMinimumSize(mWindow, size.width, size.height)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}

std::expected<kl::common::Point2<int32_t>, std::runtime_error>
Window::position() const noexcept {
  int x, y;

  if (!SDL_GetWindowPosition(mWindow, &x, &y)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return kl::common::Point2<int32_t>{x, y};
}

std::expected<void, std::runtime_error>
Window::setPosition(const kl::common::Point2<int32_t> &position) noexcept {
  if (!SDL_SetWindowPosition(mWindow, position.x, position.y)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}

std::string Window::title() const noexcept {
  return std::string(SDL_GetWindowTitle(mWindow));
}

bool Window::isResizable() const noexcept {
  Uint32 flags = SDL_GetWindowFlags(mWindow);
  return (flags & SDL_WINDOW_RESIZABLE) != 0;
}

std::expected<void, std::runtime_error>
Window::setResizable(bool resizable) noexcept {
  if (!SDL_SetWindowResizable(mWindow, resizable)) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}

std::expected<void, std::runtime_error>
Window::setTitle(std::string_view title) noexcept {
  if (!SDL_SetWindowTitle(mWindow, title.data())) {
    return std::unexpected(std::runtime_error(SDL_GetError()));
  }

  return {};
}
} // namespace kl::platform