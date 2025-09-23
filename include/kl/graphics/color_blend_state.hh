#ifndef KL_GRAPHICS_COLOR_BLEND_STATE_HH
#define KL_GRAPHICS_COLOR_BLEND_STATE_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include "color_blend_state_descriptor.hh"
#include "device_child.hh"

namespace kl::graphics {
class ColorBlendState : public DeviceChild {
private:
  ColorBlendStateDescriptor mDescriptor;

public:
  virtual ~ColorBlendState() noexcept = default;

  ColorBlendState(const ColorBlendState &) = delete;
  ColorBlendState(ColorBlendState &&) noexcept = delete;
  ColorBlendState &operator=(const ColorBlendState &) = delete;
  ColorBlendState &operator=(ColorBlendState &&) noexcept = delete;

  static std::expected<std::shared_ptr<ColorBlendState>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const ColorBlendStateDescriptor &descriptor) noexcept;

private:
  explicit ColorBlendState(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_COLOR_BLEND_STATE_HH
