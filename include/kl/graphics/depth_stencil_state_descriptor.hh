#ifndef KL_GRAPHICS_DEPTH_STENCIL_STATE_DESCRIPTOR_HH
#define KL_GRAPHICS_DEPTH_STENCIL_STATE_DESCRIPTOR_HH

#include <cstdint>

namespace kl::graphics {
/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkCompareOp.html
 */
enum class CompareOp {
  eNever,
  eLess,
  eEqual,
  eLessOrEqual,
  eGreater,
  eNotEqual,
  eGreaterOrEqual,
  eAlways,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkStencilOp.html
 */
enum class StencilOp {
  eKeep,
  eZero,
  eReplace,
  eIncrementAndClamp,
  eDecrementAndClamp,
  eInvert,
  eIncrementAndWrap,
  eDecrementAndWrap,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkStencilOpState.html
 */
struct StencilOpStateDescriptor final {
  StencilOp failOp = StencilOp::eKeep;
  StencilOp passOp = StencilOp::eKeep;
  StencilOp depthFailOp = StencilOp::eKeep;
  CompareOp compareOp = CompareOp::eAlways;
  uint32_t compareMask = 0xFFFFFFFF;
  uint32_t writeMask = 0xFFFFFFFF;
  uint32_t reference = 0;
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkPipelineDepthStencilStateCreateInfo.html
 */
struct DepthStencilStateDescriptor final {
  bool depthTestEnable = false;
  bool depthWriteEnable = true;
  CompareOp depthCompareOp = CompareOp::eLess;
  bool depthBoundsTestEnable = false;
  bool stencilTestEnable = false;
  StencilOpStateDescriptor front = {};
  StencilOpStateDescriptor back = {};
  float minDepthBounds = 0.0f;
  float maxDepthBounds = 1.0f;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_DEPTH_STENCIL_STATE_DESCRIPTOR_HH
