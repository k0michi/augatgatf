#include "kl/graphics/context.hh"

#include "kl/graphics/device.hh"

namespace kl::graphics {
void Context::setFramebuffer(
    std::shared_ptr<Framebuffer> framebuffer) noexcept {
  mState.framebuffer = std::move(framebuffer);
}

void Context::clearColor(
    std::tuple<float, float, float, float> color) noexcept {
  applyState();
  auto devicePtr = device().lock();

  if (!devicePtr) {
    return;
  }

  auto glContextOpt = currentGLContext();

  if (!glContextOpt) {
    return;
  }

  auto glContext = *glContextOpt;
  std::scoped_lock lock(*glContext);
  glContext->gladGLContext()->ClearColor(std::get<0>(color), std::get<1>(color),
                                         std::get<2>(color),
                                         std::get<3>(color));
  glContext->gladGLContext()->Clear(GL_COLOR_BUFFER_BIT);
}

void Context::clearDepthStencil(float depth, int32_t stencil) noexcept {
  applyState();
  auto devicePtr = device().lock();

  if (!devicePtr) {
    return;
  }

  auto glContextOpt = currentGLContext();

  if (!glContextOpt) {
    return;
  }

  auto glContext = *glContextOpt;
  std::scoped_lock lock(*glContext);
  glContext->gladGLContext()->ClearDepth(depth);
  glContext->gladGLContext()->ClearStencil(stencil);
  glContext->gladGLContext()->Clear(GL_DEPTH_BUFFER_BIT |
                                    GL_STENCIL_BUFFER_BIT);
}

std::expected<std::shared_ptr<Context>, std::runtime_error>
Context::create(std::shared_ptr<Device> device,
                const ContextDescriptor &descriptor) noexcept {
  auto context = std::shared_ptr<Context>(new Context(device));
  context->mDescriptor = descriptor;
  return context;
}

Context::Context(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}

void Context::applyState() noexcept {
  auto devicePtr = device().lock();

  if (!devicePtr) {
    return;
  }

  auto glContextOpt = currentGLContext();

  if (!glContextOpt) {
    return;
  }

  auto glContext = *glContextOpt;
  std::scoped_lock lock(*glContext);
  glContext->gladGLContext()->BindFramebuffer(
      GL_DRAW_FRAMEBUFFER,
      (mState.framebuffer ? mState.framebuffer->glFramebuffer() : 0));
}

std::optional<std::shared_ptr<opengl::GLContext>>
Context::currentGLContext() const noexcept {
  auto devicePtr = device().lock();

  if (!devicePtr) {
    return std::nullopt;
  }

  return (mState.framebuffer ? *(devicePtr->getOrCreateContextForWindow(
                                   mState.framebuffer->associatedWindow()))
                             : devicePtr->defaultContext());
}
} // namespace kl::graphics
