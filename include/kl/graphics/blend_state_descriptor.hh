#ifndef KL_GRAPHICS_BLEND_STATE_DESCRIPTOR_HH
#define KL_GRAPHICS_BLEND_STATE_DESCRIPTOR_HH

#include <vector>

namespace kl::graphics {
/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkBlendFactor.html
 */
enum class BlendFactor {
  eZero,
  eOne,
  eSrcColor,
  eOneMinusSrcColor,
  eDstColor,
  eSrcAlpha,
  eOneMinusSrcAlpha,
  eDstAlpha,
  eOneMinusDstAlpha,
  eConstantColor,
  eOneMinusConstantColor,
  eSrcAlphaSaturate,
  eSrc1Color,
  eOneMinusSrc1Color,
  eSrc1Alpha,
  eOneMinusSrc1Alpha,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkBlendOp.html
 */
enum class BlendOp {
  eAdd,
  eSubtract,
  eReverseSubtract,
  eMin,
  eMax,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkColorComponentFlagBits.html
 */
enum class ColorComponentFlagBits : uint32_t {
  eR = 0x1,
  eG = 0x2,
  eB = 0x4,
  eA = 0x8,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkPipelineColorBlendAttachmentState.html
 */
struct ColorBlendAttachmentStateDescriptor final {
  bool blendEnable;
  BlendFactor srcColorBlendFactor;
  BlendFactor dstColorBlendFactor;
  BlendOp colorBlendOp;
  BlendFactor srcAlphaBlendFactor;
  BlendFactor dstAlphaBlendFactor;
  BlendOp alphaBlendOp;
  ColorComponentFlagBits colorWriteMask;
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkPipelineColorBlendStateCreateInfo.html
 */
struct ColorBlendStateDescriptor final {
  std::vector<ColorBlendAttachmentStateDescriptor> attachments;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_BLEND_STATE_DESCRIPTOR_HH
