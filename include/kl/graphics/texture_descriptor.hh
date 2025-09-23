#ifndef KL_GRAPHICS_TEXTURE_DESCRIPTOR_HH
#define KL_GRAPHICS_TEXTURE_DESCRIPTOR_HH

#include <cstdint>
#include <optional>

#include "format.hh"
#include "kl/common/extent3.hh"

namespace kl::graphics {
enum class TextureType {
  e1D,
  e2D,
  e3D,
};

struct TextureDescriptor final {
  TextureType type = TextureType::e2D;
  Format format = Format::eR8G8B8A8Unorm;
  kl::common::Extent3<uint32_t> extent = {1, 1, 1};
  std::optional<uint32_t> mipLevels;
  uint32_t arrayLayers = 1;
  uint32_t samples = 1;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_TEXTURE_DESCRIPTOR_HH
