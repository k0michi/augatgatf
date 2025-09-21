#ifndef KL_GRAPHICS_RASTERIZATION_STATE_HH
#define KL_GRAPHICS_RASTERIZATION_STATE_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "rasterization_state_descriptor.hh"

namespace kl::graphics {
class RasterizationState {
private:
  RasterizationStateDescriptor mDescriptor;

public:
  virtual ~RasterizationState() noexcept = default;

  RasterizationState(const RasterizationState &) = delete;
  RasterizationState(RasterizationState &&) noexcept = delete;
  RasterizationState &operator=(const RasterizationState &) = delete;
  RasterizationState &operator=(RasterizationState &&) noexcept = delete;

  static std::expected<std::shared_ptr<RasterizationState>, std::runtime_error>
  create(const RasterizationStateDescriptor &descriptor) noexcept;

private:
  explicit RasterizationState() noexcept = default;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_RASTERIZATION_STATE_HH
