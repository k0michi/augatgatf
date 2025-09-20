#pragma once

#include <cstdint>
#include <expected>
#include <memory>
#include <stdexcept>
#include <string_view>

#include <SDL3/SDL.h>

#include "window_descriptor.hh"

namespace kl::platform {
class Window final {
private:
  SDL_Window *mWindow;

private:
  Window() = default;

public:
  ~Window();

  SDL_Window *sdlWindow() const noexcept;

  static std::expected<std::shared_ptr<Window>, std::runtime_error>
  create(const WindowDescriptor &descriptor) noexcept;
};
} // namespace kl::platform