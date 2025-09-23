#ifndef KL_GRAPHICS_BUFFER_HH
#define KL_GRAPHICS_BUFFER_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include <glad/gl.h>

#include "buffer_descriptor.hh"
#include "device_child.hh"

namespace kl::graphics {
class Device;

class Buffer : public DeviceChild {
private:
  BufferDescriptor mDescriptor;
  GLuint mBuffer = 0;

public:
  virtual ~Buffer() noexcept = default;

  Buffer(const Buffer &) = delete;
  Buffer(Buffer &&) noexcept = delete;
  Buffer &operator=(const Buffer &) = delete;
  Buffer &operator=(Buffer &&) noexcept = delete;

  inline const BufferDescriptor &descriptor() const noexcept {
    return mDescriptor;
  }
  inline GLuint glBuffer() const noexcept { return mBuffer; }

  static std::expected<std::shared_ptr<Buffer>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const BufferDescriptor &descriptor) noexcept;

private:
  explicit Buffer(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_BUFFER_HH
