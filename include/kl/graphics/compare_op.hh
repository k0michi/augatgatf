#ifndef KL_GRAPHICS_COMPARE_OP_HH
#define KL_GRAPHICS_COMPARE_OP_HH

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
} // namespace kl::graphics
#endif // KL_GRAPHICS_COMPARE_OP_HH
