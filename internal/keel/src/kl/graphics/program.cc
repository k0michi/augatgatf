#include "kl/graphics/program.hh"

#include "kl/graphics/device.hh"

namespace kl::graphics {
Program::~Program() noexcept {
  if (mProgram != 0) {
    return;
  }

  auto devicePtr = mDevice.lock();

  if (!devicePtr) {
    return;
  }

  auto contextExp = devicePtr->defaultContext();

  if (!contextExp) {
    return;
  }

  auto context = *contextExp;

  std::scoped_lock lock(*context);
  context->gladGLContext()->DeleteProgram(mProgram);
  mProgram = 0;
}

std::expected<std::shared_ptr<Program>, std::runtime_error>
Program::create(std::shared_ptr<Device> device,
                const ProgramDescriptor &descriptor) noexcept {
  auto program = std::shared_ptr<Program>(new Program(device));
  auto context = device->defaultContext();

  std::scoped_lock lock(**context);
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

  GLint linkStatus;
  (*context)->gladGLContext()->GetProgramiv(program->mProgram, GL_LINK_STATUS,
                                            &linkStatus);

  if (linkStatus != GL_TRUE) {
    GLint logLength = 0;
    (*context)->gladGLContext()->GetProgramiv(program->mProgram,
                                              GL_INFO_LOG_LENGTH, &logLength);
    std::string log(logLength, '\0');
    (*context)->gladGLContext()->GetProgramInfoLog(program->mProgram, logLength,
                                                   nullptr, log.data());
    return std::unexpected(
        std::runtime_error("Failed to link program: " + log));
  }

  return program;
}

Program::Program(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics
