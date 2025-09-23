#ifndef KL_GRAPHICS_FRAMEBUFFER_DESCRIPTOR_HH
#define KL_GRAPHICS_FRAMEBUFFER_DESCRIPTOR_HH

#include <cstdint>
#include <optional>
#include <vector>

#include "texture.hh"

namespace kl::graphics {
struct FramebufferAttachmentDescriptor final {
  std::shared_ptr<Texture> texture;
  /**
   * @brief Mipmap level.
   */
  uint32_t level = 0;
  uint32_t layer = 0;
};

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkFramebufferCreateInfo.html
 */
struct FramebufferDescriptor final {
  std::vector<FramebufferAttachmentDescriptor> colorAttachments;
  std::optional<FramebufferAttachmentDescriptor> depthAttachment;
  std::optional<FramebufferAttachmentDescriptor> stencilAttachment;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_FRAMEBUFFER_DESCRIPTOR_HH
