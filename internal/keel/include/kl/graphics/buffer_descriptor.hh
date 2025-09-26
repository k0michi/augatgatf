#ifndef KL_GRAPHICS_BUFFER_DESCRIPTOR_HH
#define KL_GRAPHICS_BUFFER_DESCRIPTOR_HH

#include <cstdint>

namespace kl::graphics {
/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkBufferUsageFlagBits.html
 */
enum class BufferUsage : uint32_t {
  /**
   * Corresponds to WGPUBufferUsage_CopySrc.
   */
  eTransferSrc = 0x00000001,
  /**
   * Corresponds to WGPUBufferUsage_CopyDst.
   */
  eTransferDst = 0x00000002,
  /**
   * Corresponds to WGPUBufferUsage_Uniform.
   */
  eUniformBuffer = 0x00000004,
  /**
   * Corresponds to WGPUBufferUsage_Storage.
   */
  eStorageBuffer = 0x00000008,
  /**
   * Corresponds to WGPUBufferUsage_Index.
   */
  eIndexBuffer = 0x00000010,
  /**
   * Corresponds to WGPUBufferUsage_Vertex.
   */
  eVertexBuffer = 0x00000020,
  /**
   * Corresponds to WGPUBufferUsage_Indirect.
   */
  eIndirectBuffer = 0x00000040,
};

constexpr BufferUsage operator|(BufferUsage lhs, BufferUsage rhs) {
  return static_cast<BufferUsage>(static_cast<uint32_t>(lhs) |
                                  static_cast<uint32_t>(rhs));
}

constexpr BufferUsage operator&(BufferUsage lhs, BufferUsage rhs) {
  return static_cast<BufferUsage>(static_cast<uint32_t>(lhs) &
                                  static_cast<uint32_t>(rhs));
}

constexpr BufferUsage operator^(BufferUsage lhs, BufferUsage rhs) {
  return static_cast<BufferUsage>(static_cast<uint32_t>(lhs) ^
                                  static_cast<uint32_t>(rhs));
}

constexpr BufferUsage &operator|=(BufferUsage &lhs, BufferUsage rhs) {
  lhs = lhs | rhs;
  return lhs;
}

constexpr BufferUsage &operator&=(BufferUsage &lhs, BufferUsage rhs) {
  lhs = lhs & rhs;
  return lhs;
}

constexpr BufferUsage &operator^=(BufferUsage &lhs, BufferUsage rhs) {
  lhs = lhs ^ rhs;
  return lhs;
}

/**
 * https://registry.khronos.org/vulkan/specs/latest/man/html/VkBufferCreateInfo.html
 */
struct BufferDescriptor final {
  uint32_t size = 0;
  BufferUsage usage = BufferUsage::eVertexBuffer | BufferUsage::eTransferDst;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_BUFFER_DESCRIPTOR_HH
