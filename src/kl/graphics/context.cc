#include "kl/graphics/context.hh"

#include "kl/graphics/device.hh"

namespace kl::graphics {
void Context::setFramebuffer(
    std::shared_ptr<Framebuffer> framebuffer) noexcept {
  mState.framebuffer = std::move(framebuffer);
}

void Context::setViewport(const std::optional<Viewport> &viewport) noexcept {
  mState.viewport = viewport;
}

const std::optional<Viewport> &Context::getViewport() const noexcept {
  return mState.viewport;
}

void Context::setScissorRect(const std::optional<ScissorRect> &rect) noexcept {
  mState.scissorRect = rect;
}

const std::optional<ScissorRect> &Context::getScissorRect() const noexcept {
  return mState.scissorRect;
}

void Context::clearColor(
    std::tuple<float, float, float, float> color) noexcept {
  applyState();
  auto devicePtr = device().lock();

  // FIXME: Error handling
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

  Viewport viewport;

  if (mState.viewport) {
    viewport = *(mState.viewport);
  } else if (mState.framebuffer) {
    auto extent = mState.framebuffer->extent();
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
  }

  glContext->gladGLContext()->Viewport(static_cast<GLint>(viewport.x),
                                       static_cast<GLint>(viewport.y),
                                       static_cast<GLsizei>(viewport.width),
                                       static_cast<GLsizei>(viewport.height));
  glContext->gladGLContext()->DepthRange(
      static_cast<GLclampd>(viewport.minDepth),
      static_cast<GLclampd>(viewport.maxDepth));

  // NOTE: Vulkan and WebGPU do not support disabling scissor test.
  glContext->gladGLContext()->Enable(GL_SCISSOR_TEST);

  ScissorRect scissorRect;
  if (mState.scissorRect) {
    scissorRect = *(mState.scissorRect);
  } else if (mState.framebuffer) {
    auto extent = mState.framebuffer->extent();
    scissorRect.offset = {0, 0};
    scissorRect.extent = {static_cast<uint32_t>(extent.width),
                          static_cast<uint32_t>(extent.height)};
  }

  glContext->gladGLContext()->Scissor(
      static_cast<GLint>(scissorRect.offset.x),
      static_cast<GLint>(scissorRect.offset.y),
      static_cast<GLsizei>(scissorRect.extent.width),
      static_cast<GLsizei>(scissorRect.extent.height));
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
