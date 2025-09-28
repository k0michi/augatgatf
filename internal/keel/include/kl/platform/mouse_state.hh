#ifndef KL_PLATFORM_MOUSE_STATE_HH
#define KL_PLATFORM_MOUSE_STATE_HH

#include <cstdint>

#include "button_state.hh"
#include "kl/common/offset2.hh"

namespace kl::platform {
struct MouseState final {
  common::Offset2<int32_t> localPosition;
  common::Offset2<int32_t> globalPosition;
  ButtonState leftButton;
  ButtonState middleButton;
  ButtonState rightButton;
  ButtonState x1Button;
  ButtonState x2Button;
  common::Offset2<float> wheel;
};
} // namespace kl::platform
#endif // KL_PLATFORM_MOUSE_STATE_HH
