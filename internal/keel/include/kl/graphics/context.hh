#ifndef KL_GRAPHICS_CONTEXT_HH
#define KL_GRAPHICS_CONTEXT_HH

#include <expected>
#include <memory>
#include <optional>
#include <span>
#include <stdexcept>
#include <vector>

#include "buffer.hh"
#include "color_blend_state.hh"
#include "context_descriptor.hh"
#include "depth_stencil_state.hh"
#include "device_child.hh"
#include "framebuffer.hh"
#include "kl/common/rectangle2.hh"
#include "opengl/gl_context.hh"
#include "primitive_topology.hh"
#include "program.hh"
#include "rasterization_state.hh"
#include "sampler.hh"
#include "scissor_rect.hh"
#include "texture.hh"
#include "vertex_input_state.hh"
#include "viewport.hh"

#include "kl/common/extent3.hh"
#include "kl/common/offset3.hh"

namespace kl::graphics {
class Device;

struct VertexBufferBinding {
  std::shared_ptr<Buffer> buffer;
  uint32_t offset = 0;
};

struct UniformBufferBinding {
  std::shared_ptr<Buffer> buffer;
  uint32_t offset = 0;
  uint32_t size = 0;
};

struct ContextState final {
  std::shared_ptr<Framebuffer> framebuffer;
  std::optional<Viewport> viewport;
  std::optional<ScissorRect> scissorRect;
  std::shared_ptr<Program> program;
  std::shared_ptr<ColorBlendState> colorBlendState;
  std::shared_ptr<RasterizationState> rasterizationState;
  std::shared_ptr<DepthStencilState> depthStencilState;
  std::shared_ptr<VertexInputState> vertexInputState;
  std::vector<std::optional<VertexBufferBinding>> vertexBufferBindings;
  std::vector<std::optional<UniformBufferBinding>> uniformBufferBindings;
  std::vector<std::shared_ptr<Texture>> textureBindings;
  std::vector<std::shared_ptr<Sampler>> samplerBindings;
};

/**
 * @brief Rendering context. Context is where users dispatch rendering commands.
 */
// TODO: Proper error handling.
class Context : public DeviceChild {
private:
  ContextDescriptor mDescriptor;
  ContextState mState;
  bool mFramebufferDirty = true;
  bool mViewportDirty = true;
  bool mScissorRectDirty = true;
  bool mProgramDirty = true;
  bool mColorBlendStateDirty = true;
  bool mRasterizationStateDirty = true;
  bool mDepthStencilStateDirty = true;
  bool mVertexInputStateDirty = true;
  bool mUniformBufferDirty = true;
  bool mTextureBindingDirty = true;
  bool mSamplerBindingDirty = true;

public:
  virtual ~Context() noexcept = default;

  Context(const Context &) = delete;
  Context(Context &&) noexcept = delete;
  Context &operator=(const Context &) = delete;
  Context &operator=(Context &&) noexcept = delete;

  inline const ContextDescriptor &descriptor() const noexcept {
    return mDescriptor;
  }

  void setFramebuffer(std::shared_ptr<Framebuffer> framebuffer) noexcept;
  void setViewport(const std::optional<Viewport> &viewport) noexcept;
  const std::optional<Viewport> &getViewport() const noexcept;
  void setScissorRect(const std::optional<ScissorRect> &rect) noexcept;
  const std::optional<ScissorRect> &getScissorRect() const noexcept;
  void setProgram(std::shared_ptr<Program> program) noexcept;
  const std::shared_ptr<Program> &getProgram() const noexcept;
  void
  setColorBlendState(std::shared_ptr<ColorBlendState> colorBlendState) noexcept;
  const std::shared_ptr<ColorBlendState> &getColorBlendState() const noexcept;
  void setRasterizationState(
      std::shared_ptr<RasterizationState> rasterizationState) noexcept;
  const std::shared_ptr<RasterizationState> &
  getRasterizationState() const noexcept;
  void setDepthStencilState(
      std::shared_ptr<DepthStencilState> depthStencilState) noexcept;
  const std::shared_ptr<DepthStencilState> &
  getDepthStencilState() const noexcept;
  void setVertexInputState(
      std::shared_ptr<VertexInputState> vertexInputState) noexcept;
  const std::shared_ptr<VertexInputState> &getVertexInputState() const noexcept;
  void setVertexBuffer(uint32_t binding, std::shared_ptr<Buffer> buffer,
                       uint32_t offset) noexcept;
  void setUniformBuffer(uint32_t binding, std::shared_ptr<Buffer> buffer,
                        uint32_t offset, uint32_t size) noexcept;
  void setTexture(uint32_t binding, std::shared_ptr<Texture> texture) noexcept;
  void setSampler(uint32_t binding, std::shared_ptr<Sampler> sampler) noexcept;

  void clearColor(std::tuple<float, float, float, float> color) noexcept;
  void clearDepthStencil(float depth, int32_t stencil) noexcept;

  void draw(PrimitiveTopology topology, uint32_t vertexCount,
            uint32_t instanceCount = 1, uint32_t firstVertex = 0,
            uint32_t firstInstance = 0) noexcept;

  void writeBuffer(std::shared_ptr<Buffer> buffer, std::uint32_t offset,
                   std::span<const std::byte> data) noexcept;
  void readBuffer(std::shared_ptr<Buffer> buffer, std::uint32_t offset,
                  std::span<std::byte> data) noexcept;
  /**
   * https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-updatesubresource
   * https://registry.khronos.org/vulkan/specs/latest/man/html/vkCmdCopyBufferToImage.html
   * https://developer.apple.com/documentation/metal/mtlblitcommandencoder/copy(from:sourceoffset:sourcebytesperrow:sourcebytesperimage:sourcesize:to:destinationslice:destinationlevel:destinationorigin:options:)?language=objc
   */
  void writeTexture(std::shared_ptr<Texture> dstTexture, int32_t dstLevel,
                    const common::Offset3<int32_t> &dstOffset,
                    const common::Extent3<uint32_t> &dstExtent,
                    std::span<const std::byte> srcData, uint32_t srcRowLength,
                    uint32_t srcImageHeight) noexcept;

  static std::expected<std::shared_ptr<Context>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const ContextDescriptor &descriptor) noexcept;

private:
  explicit Context(std::shared_ptr<Device> device) noexcept;

  void applyState() noexcept;

  std::optional<std::shared_ptr<opengl::GLContext>>
  currentGLContext() const noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_CONTEXT_HH
