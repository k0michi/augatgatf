#ifndef KL_PLATFORM_KEYBOARD_STATE_HH
#define KL_PLATFORM_KEYBOARD_STATE_HH

#include <set>
#include <stdexcept>
#include <vector>

#include <SDL3/SDL.h>

#include "button_state.hh"
#include "key_code.hh"
#include "mod_key_code.hh"
#include "scan_code.hh"

namespace kl::platform {
class KeyboardState final {
  std::vector<ButtonState> mKeyStates;

public:
  ModKeyCode mModState;

public:
  KeyboardState();

  ButtonState &operator[](ScanCode scanCode) &;
  const ButtonState &operator[](ScanCode scanCode) const &;
  ButtonState &&operator[](ScanCode scanCode) &&;
  ButtonState &operator[](KeyCode keyCode) &;
  const ButtonState &operator[](KeyCode keyCode) const &;
  ButtonState &&operator[](KeyCode keyCode) &&;

  std::size_t size() const noexcept { return mKeyStates.size(); }
};
} // namespace kl::platform
#endif // KL_PLATFORM_KEYBOARD_STATE_HH
