#ifndef KL_GRAPHICS_RASTERIZATION_STATE_DESCRIPTOR_HH
#define KL_GRAPHICS_RASTERIZATION_STATE_DESCRIPTOR_HH

namespace kl::graphics {
/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkPolygonMode.html
 */
enum class PolygonMode {
  eFill,
  eLine,
  ePoint,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkCullModeFlagBits.html
 */
enum class CullMode {
  eNone = 0x0,
  eFront = 0x1,
  eBack = 0x2,
  eFrontAndBack = 0x3,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkFrontFace.html
 */
enum class FrontFace {
  eCounterClockwise,
  eClockwise,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkPipelineRasterizationStateCreateInfo.html
 */
struct RasterizationStateDescriptor final {
  bool depthClampEnable = false;
  bool rasterizerDiscardEnable = false;
  PolygonMode polygonMode = PolygonMode::eFill;
  CullMode cullMode = CullMode::eBack;
  FrontFace frontFace = FrontFace::eCounterClockwise;
  bool depthBiasEnable = false;
  float depthBiasConstantFactor = 0.0f;
  float depthBiasClamp = 0.0f;
  float depthBiasSlopeFactor = 0.0f;
  float lineWidth = 1.0f;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_RASTERIZATION_STATE_DESCRIPTOR_HH
