#ifndef KL_GRAPHICS_PROGRAM_HH
#define KL_GRAPHICS_PROGRAM_HH

#include <expected>
#include <memory>
#include <stdexcept>
#include <vector>

#include <glad/gl.h>

#include "program_descriptor.hh"

namespace kl::graphics {
class Device;

class Program {
private:
  std::weak_ptr<Device> mDevice;
  GLuint mProgram = 0;

public:
  virtual ~Program() noexcept = default;

  Program(const Program &) = delete;
  Program(Program &&) noexcept = delete;
  Program &operator=(const Program &) = delete;
  Program &operator=(Program &&) noexcept = delete;

  static std::expected<std::shared_ptr<Program>, std::runtime_error>
  create(std::shared_ptr<Device> device,
         const ProgramDescriptor &descriptor) noexcept;

private:
  explicit Program() noexcept = default;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_PROGRAM_HH
