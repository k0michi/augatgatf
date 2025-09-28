#ifndef KL_PLATFORM_MOD_KEY_CODE_HH
#define KL_PLATFORM_MOD_KEY_CODE_HH

#include <SDL3/SDL_keycode.h>

namespace kl::platform {
enum class ModKeyCode : SDL_Keymod {
  eNone = SDL_KMOD_NONE,
  eLShift = SDL_KMOD_LSHIFT,
  eRShift = SDL_KMOD_RSHIFT,
  eLevel5 = SDL_KMOD_LEVEL5,
  eLCtrl = SDL_KMOD_LCTRL,
  eRCtrl = SDL_KMOD_RCTRL,
  eLAlt = SDL_KMOD_LALT,
  eRAlt = SDL_KMOD_RALT,
  eLGui = SDL_KMOD_LGUI,
  eRGui = SDL_KMOD_RGUI,
  eNum = SDL_KMOD_NUM,
  eCaps = SDL_KMOD_CAPS,
  eMode = SDL_KMOD_MODE,
  eScroll = SDL_KMOD_SCROLL,
  eCtrl = SDL_KMOD_CTRL,
  eShift = SDL_KMOD_SHIFT,
  eAlt = SDL_KMOD_ALT,
  eGui = SDL_KMOD_GUI,
};

constexpr ModKeyCode &operator|=(ModKeyCode &lhs, ModKeyCode rhs) {
  lhs = static_cast<ModKeyCode>(static_cast<SDL_Keymod>(lhs) |
                                static_cast<SDL_Keymod>(rhs));
  return lhs;
}

constexpr ModKeyCode operator|(ModKeyCode lhs, ModKeyCode rhs) {
  lhs |= rhs;
  return lhs;
}

constexpr ModKeyCode &operator&=(ModKeyCode &lhs, ModKeyCode rhs) {
  lhs = static_cast<ModKeyCode>(static_cast<SDL_Keymod>(lhs) &
                                static_cast<SDL_Keymod>(rhs));
  return lhs;
}

constexpr ModKeyCode operator&(ModKeyCode lhs, ModKeyCode rhs) {
  lhs &= rhs;
  return lhs;
}

constexpr ModKeyCode operator~(ModKeyCode mod) {
  return static_cast<ModKeyCode>(~static_cast<SDL_Keymod>(mod));
}

constexpr ModKeyCode &operator^=(ModKeyCode &lhs, ModKeyCode rhs) {
  lhs = static_cast<ModKeyCode>(static_cast<SDL_Keymod>(lhs) ^
                                static_cast<SDL_Keymod>(rhs));
  return lhs;
}

constexpr ModKeyCode operator^(ModKeyCode lhs, ModKeyCode rhs) {
  lhs ^= rhs;
  return lhs;
}
} // namespace kl::platform
#endif // KL_PLATFORM_MOD_KEY_CODE_HH