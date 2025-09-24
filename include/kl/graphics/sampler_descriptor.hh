#ifndef KL_GRAPHICS_SAMPLER_DESCRIPTOR_HH
#define KL_GRAPHICS_SAMPLER_DESCRIPTOR_HH

#include "kl/graphics/compare_op.hh"

namespace kl::graphics {
/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkFilter.html
 */
enum class Filter {
  eNearest,
  eLinear,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkSamplerMipmapMode.html
 */
enum class SamplerMipmapMode {
  eNearest,
  eLinear,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkSamplerAddressMode.html
 */
enum class SamplerAddressMode {
  eRepeat,
  eMirroredRepeat,
  eClampToEdge,
  eClampToBorder,
  eMirrorClampToEdge,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkBorderColor.html
 */
enum class BorderColor {
  eTransparentBlack,
  eOpaqueBlack,
  eOpaqueWhite,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkSamplerCreateInfo.html
 */
struct SamplerDescriptor final {
  Filter magFilter = Filter::eLinear;
  Filter minFilter = Filter::eLinear;
  SamplerMipmapMode mipmapMode = SamplerMipmapMode::eLinear;
  SamplerAddressMode addressModeU = SamplerAddressMode::eRepeat;
  SamplerAddressMode addressModeV = SamplerAddressMode::eRepeat;
  SamplerAddressMode addressModeW = SamplerAddressMode::eRepeat;
  float mipLodBias = 0.0f;
  bool anisotropyEnable = false;
  float maxAnisotropy = 1.0f;
  bool compareEnable = false;
  CompareOp compareOp = CompareOp::eAlways;
  float minLod = 0.0f;
  float maxLod = 1000.0f;
  BorderColor borderColor = BorderColor::eOpaqueBlack;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_SAMPLER_DESCRIPTOR_HH
