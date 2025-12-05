#ifndef KL_GRAPHICS_SHADER_HH
#define KL_GRAPHICS_SHADER_HH

#include <expected>
#include <memory>
#include <span>
#include <stdexcept>
#include <unordered_map>

#include <glad/gl.h>

#include "device_child.hh"
#include "shader_descriptor.hh"

namespace kl::graphics {
class Device;

class Shader : public DeviceChild {
private:
  GLuint mShader = 0;
  std::unordered_map<std::string, GLuint> mUniformBindings;
  std::unordered_map<std::string, GLuint> mSamplerBindings;

public:
  virtual ~Shader() noexcept;

  Shader(const Shader &) = delete;
  Shader(Shader &&) noexcept = delete;
  Shader &operator=(const Shader &) = delete;
  Shader &operator=(Shader &&) noexcept = delete;

  inline GLuint glShader() const noexcept { return mShader; }

  inline std::unordered_map<std::string, GLuint>
  uniformBindings() const noexcept {
    return mUniformBindings;
  }

  inline std::unordered_map<std::string, GLuint>
  samplerBindings() const noexcept {
    return mSamplerBindings;
  }

  static std::expected<std::shared_ptr<Shader>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const ShaderDescriptor &descriptor) noexcept;

private:
  explicit Shader(std::shared_ptr<Device> device) noexcept;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_SHADER_HH
