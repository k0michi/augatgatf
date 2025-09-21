#include "kl/graphics/rasterization_state.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<RasterizationState>, std::runtime_error>
RasterizationState::create(
    const RasterizationStateDescriptor &descriptor) noexcept {
  auto state = std::shared_ptr<RasterizationState>(new RasterizationState());
  state->mDescriptor = descriptor;
  return state;
}
} // namespace kl::graphics