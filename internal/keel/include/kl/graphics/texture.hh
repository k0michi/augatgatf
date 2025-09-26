#ifndef KL_GRAPHICS_TEXTURE_HH
#define KL_GRAPHICS_TEXTURE_HH

#include <expected>
#include <memory>
#include <stdexcept>

#include <glad/gl.h>

#include "device_child.hh"
#include "texture_descriptor.hh"

namespace kl::graphics {
class Texture : public DeviceChild {
private:
  TextureDescriptor mDescriptor;
  GLuint mTexture = 0;

public:
  virtual ~Texture() noexcept;

  Texture(const Texture &) = delete;
  Texture(Texture &&) noexcept = delete;
  Texture &operator=(const Texture &) = delete;
  Texture &operator=(Texture &&) noexcept = delete;

  inline const TextureDescriptor &descriptor() const noexcept {
    return mDescriptor;
  }

  inline GLuint glTexture() const noexcept { return mTexture; }

  static std::expected<std::shared_ptr<Texture>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const TextureDescriptor &descriptor) noexcept;

private:
  explicit Texture(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_TEXTURE_HH
