#ifndef KL_GRAPHICS_VERTEX_INPUT_STATE_HH
#define KL_GRAPHICS_VERTEX_INPUT_STATE_HH

#include <expected>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include <glad/gl.h>

#include "device_child.hh"
#include "opengl/gl_context.hh"
#include "vertex_input_state_descriptor.hh"

namespace kl::graphics {
class Device;

/**
 * @details VertexInputState is designed after
 * VkPipelineVertexInputStateCreateInfo, representing the format of vertex data.
 *  VertexInputState does not own buffers. Buffers are bound at draw time.
 */
class VertexInputState : public DeviceChild {
private:
  VertexInputStateDescriptor mDescriptor;
  std::unordered_map<std::shared_ptr<opengl::GLContext>, GLuint> mVertexArrays;
  std::unordered_map<uint32_t, VertexInputBindingDescriptor> mBindingMap;

public:
  virtual ~VertexInputState() noexcept;

  VertexInputState(const VertexInputState &) = delete;
  VertexInputState(VertexInputState &&) noexcept = delete;
  VertexInputState &operator=(const VertexInputState &) = delete;
  VertexInputState &operator=(VertexInputState &&) noexcept = delete;

  inline const VertexInputStateDescriptor &descriptor() const noexcept {
    return mDescriptor;
  }

  inline const VertexInputBindingDescriptor *
  getBindingDescriptor(uint32_t binding) const noexcept {
    auto it = mBindingMap.find(binding);
    return (it != mBindingMap.end() ? &it->second : nullptr);
  }

  /**
   * @brief Get (or create) the OpenGL vertex array object for the given
   * context.
   */
  std::expected<GLuint, std::runtime_error>
  glVertexArray(std::shared_ptr<opengl::GLContext> context) noexcept;

  static std::expected<std::shared_ptr<VertexInputState>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const VertexInputStateDescriptor &descriptor);

private:
  explicit VertexInputState(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_VERTEX_INPUT_STATE_HH
