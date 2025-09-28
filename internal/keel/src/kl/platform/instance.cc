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
    } else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
      mAccumulatedWheel.x += event.wheel.x;
      mAccumulatedWheel.y += event.wheel.y;
    }
  }
}

bool Instance::shouldQuit() const noexcept { return mShouldQuit; }

std::expected<std::shared_ptr<Window>, std::runtime_error>
Instance::createWindow(const WindowDescriptor &descriptor) noexcept {
  return Window::create(descriptor);
}

MouseState Instance::getMouseState() const noexcept {
  MouseState state;

  SDL_MouseButtonFlags buttons =
      SDL_GetMouseState(reinterpret_cast<float *>(&state.localPosition.x),
                        reinterpret_cast<float *>(&state.localPosition.y));
  SDL_GetGlobalMouseState(reinterpret_cast<float *>(&state.globalPosition.x),
                          reinterpret_cast<float *>(&state.globalPosition.y));

  state.leftButton = (buttons & SDL_BUTTON_LMASK) ? ButtonState::ePressed
                                                  : ButtonState::eReleased;
  state.middleButton = (buttons & SDL_BUTTON_MMASK) ? ButtonState::ePressed
                                                    : ButtonState::eReleased;
  state.rightButton = (buttons & SDL_BUTTON_RMASK) ? ButtonState::ePressed
                                                   : ButtonState::eReleased;
  state.x1Button = (buttons & SDL_BUTTON_X1MASK) ? ButtonState::ePressed
                                                 : ButtonState::eReleased;
  state.x2Button = (buttons & SDL_BUTTON_X2MASK) ? ButtonState::ePressed
                                                 : ButtonState::eReleased;
  state.wheel = mAccumulatedWheel;
  return state;
}

KeyboardState Instance::getKeyboardState() const noexcept {
  int numKeys = 0;
  const bool *keyStates = SDL_GetKeyboardState(&numKeys);
  KeyboardState state{};

  for (int i = 0; i < numKeys; ++i) {
    state[static_cast<ScanCode>(i)] =
        keyStates[i] ? ButtonState::ePressed : ButtonState::eReleased;
  }

  state.modState = static_cast<ModKeyCode>(SDL_GetModState());
  return state;
}

FrameAwaiter Instance::waitFrame() noexcept { return FrameAwaiter(); }
} // namespace kl::platform