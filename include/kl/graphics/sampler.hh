#ifndef KL_GRAPHICS_SAMPLER_HH
#define KL_GRAPHICS_SAMPLER_HH

#include <cstdint>
#include <expected>
#include <memory>
#include <stdexcept>

#include <glad/gl.h>

#include "kl/graphics/device_child.hh"
#include "kl/graphics/sampler_descriptor.hh"

namespace kl::graphics {
class Device;

class Sampler : public DeviceChild {
  SamplerDescriptor mDescriptor;
  GLuint mSampler = 0;

public:
  virtual ~Sampler() noexcept = default;

  Sampler(const Sampler &) = delete;
  Sampler(Sampler &&) noexcept = delete;
  Sampler &operator=(const Sampler &) = delete;
  Sampler &operator=(Sampler &&) noexcept = delete;

  inline const SamplerDescriptor &descriptor() const noexcept {
    return mDescriptor;
  }

  inline GLuint glSampler() const noexcept { return mSampler; }

  static std::expected<std::shared_ptr<Sampler>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const SamplerDescriptor &descriptor) noexcept;

private:
  explicit Sampler(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_SAMPLER_HH
