#include "kl/graphics/buffer.hh"

#include "kl/graphics/device.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<Buffer>, std::runtime_error>
Buffer::create(std::shared_ptr<Device> device,
               const BufferDescriptor &descriptor) noexcept {
  auto buffer = std::shared_ptr<Buffer>(new Buffer(device));
  buffer->mDescriptor = descriptor;
  auto context = *(device->defaultContext());

  std::scoped_lock lock(*context);
  context->gladGLContext()->GenBuffers(1, &buffer->mBuffer);

  if (buffer->mBuffer == 0) {
    return std::unexpected(std::runtime_error("Failed to create buffer"));
  }

  context->gladGLContext()->BindBuffer(GL_ARRAY_BUFFER, buffer->mBuffer);
  // In OpenGL, usage is just a hint.
  context->gladGLContext()->BufferData(GL_ARRAY_BUFFER, descriptor.size,
                                       nullptr, GL_STATIC_DRAW);
  context->gladGLContext()->BindBuffer(GL_ARRAY_BUFFER, 0);
  return buffer;
}

Buffer::Buffer(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics
