#include "kl/graphics/color_blend_state.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<ColorBlendState>, std::runtime_error>
ColorBlendState::create(std::shared_ptr<Device> device,
                        const ColorBlendStateDescriptor &descriptor) noexcept {
  auto colorBlendState =
      std::shared_ptr<ColorBlendState>(new ColorBlendState(device));
  colorBlendState->mDescriptor = descriptor;
  return colorBlendState;
}

ColorBlendState::ColorBlendState(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics
