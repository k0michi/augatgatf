#ifndef KL_GRAPHICS_VERTEX_INPUT_STATE_DESCRIPTOR_HH
#define KL_GRAPHICS_VERTEX_INPUT_STATE_DESCRIPTOR_HH

#include <vector>

#include "format.hh"

namespace kl::graphics {
/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkVertexInputRate.html
 */
enum class VertexInputRate {
  eVertex,
  eInstance,
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkVertexInputBindingDescription.html
 */
struct VertexInputBindingDescriptor final {
  uint32_t binding;
  uint32_t stride;
  VertexInputRate inputRate;
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkVertexInputAttributeDescription.html
 */
struct VertexInputAttributeDescriptor final {
  uint32_t location;
  uint32_t binding;
  Format format;
  uint32_t offset;
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkPipelineVertexInputStateCreateInfo.html
 */
struct VertexInputStateDescriptor final {
  std::vector<VertexInputBindingDescriptor> bindings;
  std::vector<VertexInputAttributeDescriptor> attributes;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_VERTEX_INPUT_STATE_DESCRIPTOR_HH
