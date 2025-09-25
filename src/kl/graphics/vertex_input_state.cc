#include "kl/graphics/vertex_input_state.hh"

#include "kl/graphics/device.hh"
#include "kl/graphics/opengl/symbol_converter.hh"

namespace kl::graphics {
VertexInputState::~VertexInputState() noexcept {
  for (auto &[context, vertexArray] : mVertexArrays) {
    if (vertexArray == 0) {
      continue;
    }

    std::scoped_lock lock(*context);
    context->gladGLContext()->DeleteVertexArrays(1, &vertexArray);
    vertexArray = 0;
  }
}

std::expected<GLuint, std::runtime_error> VertexInputState::glVertexArray(
    std::shared_ptr<opengl::GLContext> context) noexcept {
  auto it = mVertexArrays.find(context);

  if (it != mVertexArrays.end()) {
    return it->second;
  }

  std::scoped_lock lock(*context);
  GLuint vertexArray = 0;
  context->gladGLContext()->GenVertexArrays(1, &vertexArray);
  context->gladGLContext()->BindVertexArray(vertexArray);

  if (vertexArray == 0) {
    return std::unexpected(
        std::runtime_error("Failed to create vertex array object"));
  }

  for (auto &&attribute : mDescriptor.attributes) {
    context->gladGLContext()->EnableVertexAttribArray(attribute.location);
  }

  context->gladGLContext()->BindVertexArray(0);
  return mVertexArrays[context] = vertexArray;
}

std::expected<std::shared_ptr<VertexInputState>, std::runtime_error>
VertexInputState::create(std::shared_ptr<Device> device,
                         const VertexInputStateDescriptor &descriptor) {
  auto vertexInputState =
      std::shared_ptr<VertexInputState>(new VertexInputState(device));
  vertexInputState->mDescriptor = descriptor;

  for (const auto &binding : descriptor.bindings) {
    vertexInputState->mBindingMap[binding.binding] = binding;
  }

  return vertexInputState;
}

VertexInputState::VertexInputState(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics
