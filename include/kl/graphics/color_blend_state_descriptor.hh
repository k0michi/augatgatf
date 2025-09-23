#ifndef KL_GRAPHICS_COLOR_BLEND_STATE_DESCRIPTOR_HH
#define KL_GRAPHICS_COLOR_BLEND_STATE_DESCRIPTOR_HH

#include <variant>
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
enum class ColorComponent : uint32_t {
  eR = 0x1,
  eG = 0x2,
  eB = 0x4,
  eA = 0x8,
};

inline ColorComponent operator|(ColorComponent lhs, ColorComponent rhs) {
  return static_cast<ColorComponent>(static_cast<uint32_t>(lhs) |
                                     static_cast<uint32_t>(rhs));
}

inline ColorComponent operator&(ColorComponent lhs, ColorComponent rhs) {
  return static_cast<ColorComponent>(static_cast<uint32_t>(lhs) &
                                     static_cast<uint32_t>(rhs));
}

inline ColorComponent operator^(ColorComponent lhs, ColorComponent rhs) {
  return static_cast<ColorComponent>(static_cast<uint32_t>(lhs) ^
                                     static_cast<uint32_t>(rhs));
}

inline ColorComponent operator~(ColorComponent val) {
  return static_cast<ColorComponent>(~static_cast<uint32_t>(val));
}

inline ColorComponent &operator|=(ColorComponent &lhs, ColorComponent rhs) {
  lhs = lhs | rhs;
  return lhs;
}

inline ColorComponent &operator&=(ColorComponent &lhs, ColorComponent rhs) {
  lhs = lhs & rhs;
  return lhs;
}

inline ColorComponent &operator^=(ColorComponent &lhs, ColorComponent rhs) {
  lhs = lhs ^ rhs;
  return lhs;
}

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkPipelineColorBlendAttachmentState.html
 */
struct ColorBlendAttachmentStateDescriptor final {
  bool blendEnable = false;
  BlendFactor srcColorBlendFactor = BlendFactor::eOne;
  BlendFactor dstColorBlendFactor = BlendFactor::eZero;
  BlendOp colorBlendOp = BlendOp::eAdd;
  BlendFactor srcAlphaBlendFactor = BlendFactor::eOne;
  BlendFactor dstAlphaBlendFactor = BlendFactor::eZero;
  BlendOp alphaBlendOp = BlendOp::eAdd;
  ColorComponent colorWriteMask = ColorComponent::eR | ColorComponent::eG |
                                  ColorComponent::eB | ColorComponent::eA;
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkPipelineColorBlendStateCreateInfo.html
 */
struct ColorBlendStateDescriptor final {
  bool independentBlendEnable = false;
  std::vector<ColorBlendAttachmentStateDescriptor> attachments = {{}};
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_BLEND_STATE_DESCRIPTOR_HH
