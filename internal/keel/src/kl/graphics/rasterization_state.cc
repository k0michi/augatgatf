#include "kl/graphics/rasterization_state.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<RasterizationState>, std::runtime_error>
RasterizationState::create(
    std::shared_ptr<Device> device,
    const RasterizationStateDescriptor &descriptor) noexcept {
  auto state =
      std::shared_ptr<RasterizationState>(new RasterizationState(device));
  state->mDescriptor = descriptor;
  return state;
}

RasterizationState::RasterizationState(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics