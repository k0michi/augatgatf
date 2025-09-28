#include "kl/platform/keyboard_state.hh"

namespace kl::platform {
KeyboardState::KeyboardState()
    : mKeyStates(static_cast<std::size_t>(SDL_SCANCODE_COUNT),
                 ButtonState::eReleased),
      mModState(ModKeyCode::eNone) {}

ButtonState &KeyboardState::operator[](ScanCode scanCode) & {
  if (static_cast<std::size_t>(scanCode) >= mKeyStates.size()) {
    throw std::out_of_range("KeyboardState::operator[]: ScanCode out of range");
  }

  return mKeyStates[static_cast<std::size_t>(scanCode)];
}

const ButtonState &KeyboardState::operator[](ScanCode scanCode) const & {
  if (static_cast<std::size_t>(scanCode) >= mKeyStates.size()) {
    throw std::out_of_range("KeyboardState::operator[]: ScanCode out of range");
  }

  return mKeyStates[static_cast<std::size_t>(scanCode)];
}

ButtonState &&KeyboardState::operator[](ScanCode scanCode) && {
  if (static_cast<std::size_t>(scanCode) >= mKeyStates.size()) {
    throw std::out_of_range("KeyboardState::operator[]: ScanCode out of range");
  }

  return std::move(mKeyStates[static_cast<std::size_t>(scanCode)]);
}

ButtonState &KeyboardState::operator[](KeyCode keyCode) & {
  if (auto scanCode =
          SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(keyCode), nullptr);
      scanCode != SDL_SCANCODE_UNKNOWN) {
    return (*this)[static_cast<ScanCode>(scanCode)];
  } else {
    throw std::out_of_range("KeyboardState::operator[]: KeyCode out of range");
  }
}

const ButtonState &KeyboardState::operator[](KeyCode keyCode) const & {
  if (auto scanCode =
          SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(keyCode), nullptr);
      scanCode != SDL_SCANCODE_UNKNOWN) {
    return (*this)[static_cast<ScanCode>(scanCode)];
  } else {
    throw std::out_of_range("KeyboardState::operator[]: KeyCode out of range");
  }
}

ButtonState &&KeyboardState::operator[](KeyCode keyCode) && {
  if (auto scanCode =
          SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(keyCode), nullptr);
      scanCode != SDL_SCANCODE_UNKNOWN) {
    return std::move((*this)[static_cast<ScanCode>(scanCode)]);
  } else {
    throw std::out_of_range("KeyboardState::operator[]: KeyCode out of range");
  }
}
} // namespace kl::platform