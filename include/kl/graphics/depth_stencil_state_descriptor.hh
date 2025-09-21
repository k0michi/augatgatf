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
  StencilOp failOp;
  StencilOp passOp;
  StencilOp depthFailOp;
  StencilOp compareOp;
  uint32_t compareMask;
  uint32_t writeMask;
  uint32_t reference;
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkPipelineDepthStencilStateCreateInfo.html
 */
struct DepthStencilStateDescriptor final {
  bool depthTestEnable;
  bool depthWriteEnable;
  CompareOp depthCompareOp;
  bool depthBoundsTestEnable;
  bool stencilTestEnable;
  StencilOpStateDescriptor front;
  StencilOpStateDescriptor back;
  float minDepthBounds;
  float maxDepthBounds;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_DEPTH_STENCIL_STATE_DESCRIPTOR_HH
