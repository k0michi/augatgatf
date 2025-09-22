#include "kl/graphics/shader.hh"

#include <spirv_glsl.hpp>

#include "kl/graphics/device.hh"

namespace kl::graphics {
Shader::~Shader() noexcept {
  auto device = mDevice.lock();

  if (device) {
    auto context = device->defaultContext();

    if (context) {
      std::scoped_lock lock(**context);
      (*context)->gladGLContext()->DeleteShader(mShader);
    }
  }
}

std::expected<std::shared_ptr<Shader>, std::runtime_error>
Shader::create(std::shared_ptr<Device> device,
               const ShaderDescriptor &descriptor) noexcept {
  auto shader = std::shared_ptr<Shader>(new Shader());

  if (descriptor.code.size() % 4 != 0) {
    return std::unexpected(
        std::runtime_error("Shader code size is not a multiple of 4."));
  }

  spirv_cross::CompilerGLSL glsl(std::vector<uint32_t>(
      reinterpret_cast<const uint32_t *>(descriptor.code.data()),
      reinterpret_cast<const uint32_t *>(descriptor.code.data()) +
          descriptor.code.size() / 4));
  spv::ExecutionModel model;

  switch (descriptor.stage) {
  case ShaderStage::eVertex:
    model = spv::ExecutionModelVertex;
    break;
  case ShaderStage::eFragment:
    model = spv::ExecutionModelFragment;
    break;
  }

  glsl.set_entry_point(descriptor.entryPoint, model);
  glsl.set_common_options({.version =
#ifdef __EMSCRIPTEN__
                               300
#else
                               450
#endif
                           ,
                           .es =
#ifdef __EMSCRIPTEN__
                               true
#else
                               false
#endif
  });
  auto constants = glsl.get_specialization_constants();

  for (auto &&specConst : descriptor.specializationConstants) {
    for (auto &&constant : constants) {
      if (constant.constant_id == specConst.id) {
        auto &spirvConst = glsl.get_constant(constant.id);

        if (std::holds_alternative<int32_t>(specConst.value)) {
          spirvConst.m.c[0].r[0].i32 = std::get<int32_t>(specConst.value);
        } else if (std::holds_alternative<uint32_t>(specConst.value)) {
          spirvConst.m.c[0].r[0].u32 = std::get<uint32_t>(specConst.value);
        } else if (std::holds_alternative<float>(specConst.value)) {
          spirvConst.m.c[0].r[0].f32 = std::get<float>(specConst.value);
        } else if (std::holds_alternative<double>(specConst.value)) {
          spirvConst.m.c[0].r[0].f64 = std::get<double>(specConst.value);
        } else if (std::holds_alternative<bool>(specConst.value)) {
          spirvConst.m.c[0].r[0].i32 = std::get<bool>(specConst.value);
        }
        goto next;
      }
    }

    return std::unexpected(std::runtime_error(
        "Specialization constant with ID " + std::to_string(specConst.id) +
        " not found in shader code."));

  next:;
  }

  std::string source;

  try {
    source = glsl.compile();
  } catch (const std::exception &e) {
    return std::unexpected(std::runtime_error("Failed to compile shader: " +
                                              std::string(e.what())));
  }

  auto context = device->defaultContext();

  if (!context) {
    return std::unexpected(
        std::runtime_error("No default context available in device."));
  }

  {
    std::scoped_lock lock(**context);
    GLenum shaderType;

    switch (descriptor.stage) {
    case ShaderStage::eVertex:
      shaderType = GL_VERTEX_SHADER;
      break;
    case ShaderStage::eFragment:
      shaderType = GL_FRAGMENT_SHADER;
      break;
    }

    shader->mShader = (*context)->gladGLContext()->CreateShader(shaderType);

    if (shader->mShader == 0) {
      return std::unexpected(
          std::runtime_error("Failed to create shader object."));
    }

    const char *sourceCStr = source.c_str();
    (*context)->gladGLContext()->ShaderSource(shader->mShader, 1, &sourceCStr,
                                              nullptr);
    (*context)->gladGLContext()->CompileShader(shader->mShader);

    GLint compileStatus;
    (*context)->gladGLContext()->GetShaderiv(shader->mShader, GL_COMPILE_STATUS,
                                             &compileStatus);

    if (compileStatus != GL_TRUE) {
      GLint logLength = 0;
      (*context)->gladGLContext()->GetShaderiv(shader->mShader,
                                               GL_INFO_LOG_LENGTH, &logLength);
      std::string log(logLength, '\0');
      (*context)->gladGLContext()->GetShaderInfoLog(shader->mShader, logLength,
                                                    nullptr, log.data());
      return std::unexpected(
          std::runtime_error("Failed to compile shader: " + log));
    }
  }

  shader->mDevice = device;
  return shader;
}
} // namespace kl::graphics