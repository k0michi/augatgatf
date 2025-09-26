#include "kl/graphics/depth_stencil_state.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<DepthStencilState>, std::runtime_error>
DepthStencilState::create(
    std::shared_ptr<Device> device,
    const DepthStencilStateDescriptor &descriptor) noexcept {
  auto depthStencilState = std::shared_ptr<DepthStencilState>(
      new DepthStencilState(std::move(device)));
  depthStencilState->mDescriptor = descriptor;
  return depthStencilState;
}

DepthStencilState::DepthStencilState(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics
