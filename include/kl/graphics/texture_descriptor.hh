#ifndef KL_GRAPHICS_TEXTURE_DESCRIPTOR_HH
#define KL_GRAPHICS_TEXTURE_DESCRIPTOR_HH

#include <cstdint>
#include <optional>

#include "format.hh"
#include "kl/common/extent3.hh"

namespace kl::graphics {
/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkImageType.html
 */
enum class TextureType {
  e1D,
  e2D,
  e3D,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkImageUsageFlagBits.html
 */
enum class TextureUsage : uint32_t {
  /**
   * Corresponds to WGPUTextureUsage_CopySrc.
   */
  eTransferSrc = 0x00000001,
  /**
   * Corresponds to WGPUTextureUsage_CopyDst.
   */
  eTransferDst = 0x00000002,
  /**
   * Corresponds to WGPUTextureUsage_TextureBinding.
   */
  eSampled = 0x00000004,
  /**
   * Corresponds to WGPUTextureUsage_StorageBinding.
   */
  eStorage = 0x00000008,
  /**
   * Corresponds to WGPUTextureUsage_RenderAttachment.
   */
  eColorAttachment = 0x00000010,
  /**
   * Corresponds to WGPUTextureUsage_RenderAttachment.
   */
  eDepthStencilAttachment = 0x00000020,
};

constexpr TextureUsage operator|(TextureUsage lhs, TextureUsage rhs) {
  return static_cast<TextureUsage>(static_cast<uint32_t>(lhs) |
                                   static_cast<uint32_t>(rhs));
}

constexpr TextureUsage operator&(TextureUsage lhs, TextureUsage rhs) {
  return static_cast<TextureUsage>(static_cast<uint32_t>(lhs) &
                                   static_cast<uint32_t>(rhs));
}

constexpr TextureUsage operator^(TextureUsage lhs, TextureUsage rhs) {
  return static_cast<TextureUsage>(static_cast<uint32_t>(lhs) ^
                                   static_cast<uint32_t>(rhs));
}

constexpr TextureUsage operator~(TextureUsage usage) {
  return static_cast<TextureUsage>(~static_cast<uint32_t>(usage));
}

constexpr TextureUsage &operator|=(TextureUsage &lhs, TextureUsage rhs) {
  lhs = lhs | rhs;
  return lhs;
}

constexpr TextureUsage &operator&=(TextureUsage &lhs, TextureUsage rhs) {
  lhs = lhs & rhs;
  return lhs;
}

constexpr TextureUsage &operator^=(TextureUsage &lhs, TextureUsage rhs) {
  lhs = lhs ^ rhs;
  return lhs;
}

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkImageCreateInfo.html
 */
struct TextureDescriptor final {
  TextureType type = TextureType::e2D;
  Format format = Format::eR8G8B8A8Unorm;
  kl::common::Extent3<uint32_t> extent = {1, 1, 1};
  std::optional<uint32_t> mipLevels;
  uint32_t arrayLayers = 1;
  uint32_t samples = 1;
  TextureUsage usage = TextureUsage::eSampled | TextureUsage::eTransferDst;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_TEXTURE_DESCRIPTOR_HH
