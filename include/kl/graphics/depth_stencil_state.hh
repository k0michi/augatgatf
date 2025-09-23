#ifndef KL_GRAPHICS_DEPTH_STENCIL_STATE_HH
#define KL_GRAPHICS_DEPTH_STENCIL_STATE_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "depth_stencil_state_descriptor.hh"
#include "device_child.hh"

namespace kl::graphics {
class DepthStencilState : public DeviceChild {
private:
  DepthStencilStateDescriptor mDescriptor;

public:
  virtual ~DepthStencilState() noexcept = default;

  DepthStencilState(const DepthStencilState &) = delete;
  DepthStencilState(DepthStencilState &&) noexcept = delete;
  DepthStencilState &operator=(const DepthStencilState &) = delete;
  DepthStencilState &operator=(DepthStencilState &&) noexcept = delete;

  inline const DepthStencilStateDescriptor &descriptor() const noexcept {
    return mDescriptor;
  }

  static std::expected<std::shared_ptr<DepthStencilState>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const DepthStencilStateDescriptor &descriptor) noexcept;

private:
  explicit DepthStencilState(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_DEPTH_STENCIL_STATE_HH
