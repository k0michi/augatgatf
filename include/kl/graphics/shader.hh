#ifndef KL_GRAPHICS_SHADER_HH
#define KL_GRAPHICS_SHADER_HH

#include <expected>
#include <memory>
#include <span>
#include <stdexcept>

#include <glad/gl.h>

#include "shader_descriptor.hh"

namespace kl::graphics {
class Device;

class Shader {
private:
  std::weak_ptr<Device> mDevice;
  GLuint mShader = 0;

public:
  virtual ~Shader() noexcept;

  Shader(const Shader &) = delete;
  Shader(Shader &&) noexcept = delete;
  Shader &operator=(const Shader &) = delete;
  Shader &operator=(Shader &&) noexcept = delete;

  static std::expected<std::shared_ptr<Shader>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const ShaderDescriptor &descriptor) noexcept;

private:
  explicit Shader() noexcept = default;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_SHADER_HH
