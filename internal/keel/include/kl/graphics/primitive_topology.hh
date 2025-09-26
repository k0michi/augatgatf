#ifndef KL_GRAPHICS_PRIMITIVE_TOPOLOGY_HH
#define KL_GRAPHICS_PRIMITIVE_TOPOLOGY_HH

namespace kl::graphics {
/**
 * https://registry.khronos.org/VulkanSC/specs/1.0-extensions/man/html/VkPrimitiveTopology.html
 */
enum class PrimitiveTopology {
  ePointList,
  eLineList,
  eLineStrip,
  eTriangleList,
  eTriangleStrip,
  eTriangleFan,
  eLineListWithAdjacency,
  eLineStripWithAdjacency,
  eTriangleListWithAdjacency,
  eTriangleStripWithAdjacency,
  ePatchList,
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_PRIMITIVE_TOPOLOGY_HH
