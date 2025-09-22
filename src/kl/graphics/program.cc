#include "kl/graphics/program.hh"

#include "kl/graphics/device.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<Program>, std::runtime_error>
Program::create(std::shared_ptr<Device> device,
                const ProgramDescriptor &descriptor) noexcept {
  auto program = std::shared_ptr<Program>(new Program());
  auto context = device->defaultContext();

  std::scoped_lock lock(**context);
  program->mDevice = device;
  program->mProgram = (*context)->gladGLContext()->CreateProgram();

  for (const auto &shader : descriptor.shaders) {
    if (!shader) {
      return std::unexpected(
          std::runtime_error("Shader in ProgramDescriptor is null."));
    }

    (*context)->gladGLContext()->AttachShader(program->mProgram,
                                              shader->glShader());
  }

  (*context)->gladGLContext()->LinkProgram(program->mProgram);
  return program;
}
} // namespace kl::graphics
