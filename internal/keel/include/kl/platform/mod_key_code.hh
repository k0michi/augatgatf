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

ModKeyCode operator|(ModKeyCode a, ModKeyCode b) noexcept {
  return static_cast<ModKeyCode>(static_cast<SDL_Keymod>(a) |
                                 static_cast<SDL_Keymod>(b));
}

ModKeyCode &operator|=(ModKeyCode &a, ModKeyCode b) noexcept {
  a = a | b;
  return a;
}

ModKeyCode operator&(ModKeyCode a, ModKeyCode b) noexcept {
  return static_cast<ModKeyCode>(static_cast<SDL_Keymod>(a) &
                                 static_cast<SDL_Keymod>(b));
}

ModKeyCode &operator&=(ModKeyCode &a, ModKeyCode b) noexcept {
  a = a & b;
  return a;
}

ModKeyCode operator~(ModKeyCode a) noexcept {
  return static_cast<ModKeyCode>(~static_cast<SDL_Keymod>(a));
}

ModKeyCode &operator^=(ModKeyCode &a, ModKeyCode b) noexcept {
  a = a ^ b;
  return a;
}

ModKeyCode operator^(ModKeyCode a, ModKeyCode b) noexcept {
  return static_cast<ModKeyCode>(static_cast<SDL_Keymod>(a) ^
                                 static_cast<SDL_Keymod>(b));
}
} // namespace kl::platform
#endif // KL_PLATFORM_MOD_KEY_CODE_HH