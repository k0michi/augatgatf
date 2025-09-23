#include "kl/graphics/framebuffer.hh"

#include "kl/graphics/device.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<Framebuffer>, std::runtime_error>
Framebuffer::create(std::shared_ptr<Device> device,
                    const FramebufferDescriptor &descriptor) noexcept {
  auto framebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(device));
  framebuffer->mDescriptor = descriptor;
  auto context = *(device->defaultContext());
  std::scoped_lock lock(*context);

  context->gladGLContext()->GenFramebuffers(1, &framebuffer->mFramebuffer);

  if (framebuffer->mFramebuffer == 0) {
    return std::unexpected(std::runtime_error("Failed to create framebuffer"));
  }

  context->gladGLContext()->BindFramebuffer(GL_FRAMEBUFFER,
                                            framebuffer->mFramebuffer);

  std::vector<GLenum> drawBuffers;
  for (size_t i = 0; i < descriptor.colorAttachments.size(); ++i) {
    const auto &attachment = descriptor.colorAttachments[i];

    if (!attachment.texture) {
      return std::unexpected(
          std::runtime_error("Color attachment texture is null"));
    }

    // TODO: Implement other texture types.
    if (attachment.texture->descriptor().type == TextureType::e2D) {
      context->gladGLContext()->FramebufferTexture2D(
          GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i),
          GL_TEXTURE_2D, attachment.texture->glTexture(), attachment.level);
    } else {
      return std::unexpected(
          std::runtime_error("Unsupported texture type for color attachment"));
    }
  }

  if (descriptor.depthAttachment) {
    const auto &attachment = descriptor.depthAttachment.value();

    if (!attachment.texture) {
      return std::unexpected(
          std::runtime_error("Depth attachment texture is null"));
    }

    if (attachment.texture->descriptor().type == TextureType::e2D) {
      context->gladGLContext()->FramebufferTexture2D(
          GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
          attachment.texture->glTexture(), attachment.level);
    }
  }

  if (descriptor.stencilAttachment) {
    const auto &attachment = descriptor.stencilAttachment.value();

    if (!attachment.texture) {
      return std::unexpected(
          std::runtime_error("Stencil attachment texture is null"));
    }

    if (attachment.texture->descriptor().type == TextureType::e2D) {
      context->gladGLContext()->FramebufferTexture2D(
          GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
          attachment.texture->glTexture(), attachment.level);
    }
  }

  return framebuffer;
}

std::expected<std::shared_ptr<Framebuffer>, std::runtime_error>
Framebuffer::createDefault(std::shared_ptr<Device> device,
                           std::shared_ptr<platform::Window> window) noexcept {
  auto framebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(device));
  framebuffer->mAssociatedWindow = window;
  framebuffer->mFramebuffer = 0;
  return framebuffer;
}

Framebuffer::Framebuffer(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics