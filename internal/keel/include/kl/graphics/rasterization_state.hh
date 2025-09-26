#ifndef KL_GRAPHICS_RASTERIZATION_STATE_HH
#define KL_GRAPHICS_RASTERIZATION_STATE_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "kl/graphics/device_child.hh"
#include "rasterization_state_descriptor.hh"

namespace kl::graphics {
class RasterizationState : public DeviceChild {
private:
  RasterizationStateDescriptor mDescriptor;

public:
  virtual ~RasterizationState() noexcept = default;

  RasterizationState(const RasterizationState &) = delete;
  RasterizationState(RasterizationState &&) noexcept = delete;
  RasterizationState &operator=(const RasterizationState &) = delete;
  RasterizationState &operator=(RasterizationState &&) noexcept = delete;

  inline const RasterizationStateDescriptor &descriptor() const noexcept {
    return mDescriptor;
  }

  static std::expected<std::shared_ptr<RasterizationState>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const RasterizationStateDescriptor &descriptor) noexcept;

private:
  explicit RasterizationState(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_RASTERIZATION_STATE_HH
