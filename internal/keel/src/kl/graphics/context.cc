#include "kl/graphics/context.hh"

#include "kl/graphics/device.hh"
#include "kl/graphics/opengl/symbol_converter.hh"

namespace kl::graphics {
void Context::setFramebuffer(
    std::shared_ptr<Framebuffer> framebuffer) noexcept {
  mState.framebuffer = std::move(framebuffer);
  mFramebufferDirty = true;
}

void Context::setViewport(const std::optional<Viewport> &viewport) noexcept {
  mState.viewport = viewport;
  mViewportDirty = true;
}

const std::optional<Viewport> &Context::getViewport() const noexcept {
  return mState.viewport;
}

void Context::setScissorRect(const std::optional<ScissorRect> &rect) noexcept {
  mState.scissorRect = rect;
  mScissorRectDirty = true;
}

const std::optional<ScissorRect> &Context::getScissorRect() const noexcept {
  return mState.scissorRect;
}

void Context::setProgram(std::shared_ptr<Program> program) noexcept {
  mState.program = std::move(program);
  mProgramDirty = true;
}

const std::shared_ptr<Program> &Context::getProgram() const noexcept {
  return mState.program;
}

void Context::setColorBlendState(
    std::shared_ptr<ColorBlendState> colorBlendState) noexcept {
  mState.colorBlendState = std::move(colorBlendState);
  mColorBlendStateDirty = true;
}

const std::shared_ptr<ColorBlendState> &
Context::getColorBlendState() const noexcept {
  return mState.colorBlendState;
}

void Context::setRasterizationState(
    std::shared_ptr<RasterizationState> rasterizationState) noexcept {
  mState.rasterizationState = std::move(rasterizationState);
  mRasterizationStateDirty = true;
}

const std::shared_ptr<RasterizationState> &
Context::getRasterizationState() const noexcept {
  return mState.rasterizationState;
}

void Context::setDepthStencilState(
    std::shared_ptr<DepthStencilState> depthStencilState) noexcept {
  mState.depthStencilState = std::move(depthStencilState);
  mDepthStencilStateDirty = true;
}

const std::shared_ptr<DepthStencilState> &
Context::getDepthStencilState() const noexcept {
  return mState.depthStencilState;
}

void Context::setVertexInputState(
    std::shared_ptr<VertexInputState> vertexInputState) noexcept {
  mState.vertexInputState = std::move(vertexInputState);
  mVertexInputStateDirty = true;
}

const std::shared_ptr<VertexInputState> &
Context::getVertexInputState() const noexcept {
  return mState.vertexInputState;
}

void Context::setVertexBuffer(uint32_t binding, std::shared_ptr<Buffer> buffer,
                              uint32_t offset) noexcept {
  if (mState.vertexBufferBinding.size() <= binding) {
    mState.vertexBufferBinding.resize(binding + 1);
  }

  mState.vertexBufferBinding[binding] = {std::move(buffer), offset};
  mVertexInputStateDirty = true;
}

void Context::setUniformBuffer(uint32_t binding, std::shared_ptr<Buffer> buffer,
                               uint32_t offset, uint32_t size) noexcept {
  if (mState.uniformBufferBinding.size() <= binding) {
    mState.uniformBufferBinding.resize(binding + 1);
  }

  mState.uniformBufferBinding[binding] = {std::move(buffer), offset, size};
  mUniformBufferDirty = true;
}

void Context::setTexture(uint32_t binding,
                         std::shared_ptr<Texture> texture) noexcept {
  if (mState.textureBinding.size() <= binding) {
    mState.textureBinding.resize(binding + 1);
  }

  mState.textureBinding[binding] = std::move(texture);
  mTextureBindingDirty = true;
}

void Context::setSampler(uint32_t binding,
                         std::shared_ptr<Sampler> sampler) noexcept {
  if (mState.samplerBinding.size() <= binding) {
    mState.samplerBinding.resize(binding + 1);
  }

  mState.samplerBinding[binding] = std::move(sampler);
  mTextureBindingDirty = true;
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

void Context::draw(PrimitiveTopology topology, uint32_t vertexCount,
                   uint32_t instanceCount, uint32_t firstVertex,
                   uint32_t firstInstance) noexcept {
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

  GLenum mode = opengl::SymbolConverter::toGLDrawMode(topology);

  if (firstInstance == 0) {
    if (instanceCount == 1) {
      glContext->gladGLContext()->DrawArrays(mode,
                                             static_cast<GLint>(firstVertex),
                                             static_cast<GLsizei>(vertexCount));
    } else {
      glContext->gladGLContext()->DrawArraysInstanced(
          mode, static_cast<GLint>(firstVertex),
          static_cast<GLsizei>(vertexCount),
          static_cast<GLsizei>(instanceCount));
    }
  } else {
    glContext->gladGLContext()->DrawArraysInstancedBaseInstance(
        mode, static_cast<GLint>(firstVertex),
        static_cast<GLsizei>(vertexCount), static_cast<GLsizei>(instanceCount),
        static_cast<GLuint>(firstInstance));
  }
}

std::expected<std::shared_ptr<Context>, std::runtime_error>
Context::create(std::shared_ptr<Device> device,
                const ContextDescriptor &descriptor) noexcept {
  auto context = std::shared_ptr<Context>(new Context(device));
  context->mDescriptor = descriptor;
  return context;
}

void Context::writeBuffer(std::shared_ptr<Buffer> buffer, std::uint32_t offset,
                          std::span<const std::byte> data) noexcept {
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
  glContext->gladGLContext()->BindBuffer(GL_ARRAY_BUFFER, buffer->glBuffer());
  glContext->gladGLContext()->BufferSubData(
      GL_ARRAY_BUFFER, static_cast<GLintptr>(offset),
      static_cast<GLsizeiptr>(data.size_bytes()), data.data());
  glContext->gladGLContext()->BindBuffer(GL_ARRAY_BUFFER, 0);
}

void Context::readBuffer(std::shared_ptr<Buffer> buffer, std::uint32_t offset,
                         std::span<std::byte> data) noexcept {
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
  glContext->gladGLContext()->BindBuffer(GL_ARRAY_BUFFER, buffer->glBuffer());
  glContext->gladGLContext()->GetBufferSubData(
      GL_ARRAY_BUFFER, static_cast<GLintptr>(offset),
      static_cast<GLsizeiptr>(data.size_bytes()), data.data());
  glContext->gladGLContext()->BindBuffer(GL_ARRAY_BUFFER, 0);
}

void Context::writeTexture(std::shared_ptr<Texture> dstTexture,
                           int32_t dstLevel,
                           const common::Offset3<int32_t> &dstOffset,
                           const common::Extent3<uint32_t> &dstExtent,
                           std::span<const std::byte> srcData,
                           uint32_t srcRowLength,
                           uint32_t srcImageHeight) noexcept {
  auto devicePtr = device().lock();

  if (!devicePtr) {
    return;
  }

  auto glContextOpt = currentGLContext();

  if (!glContextOpt) {
    return;
  }

  auto format =
      opengl::SymbolConverter::toGLFormat(dstTexture->descriptor().format);

  auto glContext = *glContextOpt;
  std::scoped_lock lock(*glContext);
  glContext->gladGLContext()->PixelStorei(GL_UNPACK_ROW_LENGTH, srcRowLength);
  glContext->gladGLContext()->PixelStorei(GL_UNPACK_IMAGE_HEIGHT,
                                          srcImageHeight);

  if (dstTexture->descriptor().type == TextureType::e2D) {
    glContext->gladGLContext()->BindTexture(GL_TEXTURE_2D,
                                            dstTexture->glTexture());
    glContext->gladGLContext()->TexSubImage2D(
        GL_TEXTURE_2D, dstLevel, dstOffset.x, dstOffset.y,
        static_cast<GLsizei>(dstExtent.width),
        static_cast<GLsizei>(dstExtent.height), std::get<1>(format),
        std::get<2>(format), srcData.data());
    glContext->gladGLContext()->BindTexture(GL_TEXTURE_2D, 0);
  } else if (dstTexture->descriptor().type == TextureType::e3D) {
    glContext->gladGLContext()->BindTexture(GL_TEXTURE_3D,
                                            dstTexture->glTexture());
    glContext->gladGLContext()->TexSubImage3D(
        GL_TEXTURE_3D, dstLevel, dstOffset.x, dstOffset.y, dstOffset.z,
        static_cast<GLsizei>(dstExtent.width),
        static_cast<GLsizei>(dstExtent.height),
        static_cast<GLsizei>(dstExtent.depth), std::get<1>(format),
        std::get<2>(format), srcData.data());
    glContext->gladGLContext()->BindTexture(GL_TEXTURE_3D, 0);
  }

  glContext->gladGLContext()->PixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glContext->gladGLContext()->PixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
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

  // Framebuffer

  if (mFramebufferDirty) {
    glContext->gladGLContext()->BindFramebuffer(
        GL_DRAW_FRAMEBUFFER,
        (mState.framebuffer ? mState.framebuffer->glFramebuffer() : 0));
    mFramebufferDirty = false;
  }

  // Viewport

  if (mViewportDirty) {
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
    glContext->gladGLContext()->DepthRangef(viewport.minDepth,
                                            viewport.maxDepth);
    mViewportDirty = false;
  }

  // Scissor Rect

  if (mScissorRectDirty) {
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
    mScissorRectDirty = false;
  }

  // Program

  if (mProgramDirty) {
    glContext->gladGLContext()->UseProgram(
        (mState.program ? mState.program->glProgram() : 0));
    mProgramDirty = false;
  }

  // Color Blend State

  if (mColorBlendStateDirty) {
    ColorBlendStateDescriptor descriptor;

    if (mState.colorBlendState) {
      descriptor = mState.colorBlendState->descriptor();
    } else {
      descriptor = ColorBlendStateDescriptor{};
    }

    if (descriptor.independentBlendEnable) {
      // TODO: Check whether independent blending is supported.

      for (uint32_t i = 0; i < descriptor.attachments.size(); i++) {
        if (descriptor.attachments[i].blendEnable) {
          glContext->gladGLContext()->Enablei(GL_BLEND, i);
        } else {
          glContext->gladGLContext()->Disablei(GL_BLEND, i);
        }

        glContext->gladGLContext()->BlendFuncSeparatei(
            i,
            opengl::SymbolConverter::toGLBlendFunc(
                descriptor.attachments[i].srcColorBlendFactor),
            opengl::SymbolConverter::toGLBlendFunc(
                descriptor.attachments[i].dstColorBlendFactor),
            opengl::SymbolConverter::toGLBlendFunc(
                descriptor.attachments[i].srcAlphaBlendFactor),
            opengl::SymbolConverter::toGLBlendFunc(
                descriptor.attachments[i].dstAlphaBlendFactor));
        glContext->gladGLContext()->BlendEquationSeparatei(
            i,
            opengl::SymbolConverter::toGLBlendEquation(
                descriptor.attachments[i].colorBlendOp),
            opengl::SymbolConverter::toGLBlendEquation(
                descriptor.attachments[i].alphaBlendOp));
        auto colorMask = opengl::SymbolConverter::toGLColorMask(
            descriptor.attachments[i].colorWriteMask);
        glContext->gladGLContext()->ColorMaski(
            i, std::get<0>(colorMask), std::get<1>(colorMask),
            std::get<2>(colorMask), std::get<3>(colorMask));
      }
    } else {
      if (descriptor.attachments[0].blendEnable) {
        glContext->gladGLContext()->Enable(GL_BLEND);
      } else {
        glContext->gladGLContext()->Disable(GL_BLEND);
      }

      glContext->gladGLContext()->BlendFuncSeparate(
          opengl::SymbolConverter::toGLBlendFunc(
              descriptor.attachments[0].srcColorBlendFactor),
          opengl::SymbolConverter::toGLBlendFunc(
              descriptor.attachments[0].dstColorBlendFactor),
          opengl::SymbolConverter::toGLBlendFunc(
              descriptor.attachments[0].srcAlphaBlendFactor),
          opengl::SymbolConverter::toGLBlendFunc(
              descriptor.attachments[0].dstAlphaBlendFactor));
      glContext->gladGLContext()->BlendEquationSeparate(
          opengl::SymbolConverter::toGLBlendEquation(
              descriptor.attachments[0].colorBlendOp),
          opengl::SymbolConverter::toGLBlendEquation(
              descriptor.attachments[0].alphaBlendOp));
      auto colorMask = opengl::SymbolConverter::toGLColorMask(
          descriptor.attachments[0].colorWriteMask);
      glContext->gladGLContext()->ColorMask(
          std::get<0>(colorMask), std::get<1>(colorMask),
          std::get<2>(colorMask), std::get<3>(colorMask));
    }

    mColorBlendStateDirty = false;
  }

  // Rasterization State

  if (mRasterizationStateDirty) {
    RasterizationStateDescriptor descriptor;

    if (mState.rasterizationState) {
      descriptor = mState.rasterizationState->descriptor();
    } else {
      descriptor = RasterizationStateDescriptor{};
    }

    if (descriptor.depthClampEnable) {
      glContext->gladGLContext()->Enable(GL_DEPTH_CLAMP);
    } else {
      glContext->gladGLContext()->Disable(GL_DEPTH_CLAMP);
    }

    if (descriptor.rasterizerDiscardEnable) {
      glContext->gladGLContext()->Enable(GL_RASTERIZER_DISCARD);
    } else {
      glContext->gladGLContext()->Disable(GL_RASTERIZER_DISCARD);
    }

    // TODO: Report error if polygon mode other than FILL is used on WebGL.
#ifndef __EMSCRIPTEN__
    glContext->gladGLContext()->PolygonMode(
        GL_FRONT_AND_BACK,
        opengl::SymbolConverter::toGLPolygonMode(descriptor.polygonMode));
#endif

    if (descriptor.cullMode == CullMode::eNone) {
      glContext->gladGLContext()->Disable(GL_CULL_FACE);
    } else {
      glContext->gladGLContext()->Enable(GL_CULL_FACE);
      glContext->gladGLContext()->CullFace(
          *opengl::SymbolConverter::toGLCullMode(descriptor.cullMode));
    }

    glContext->gladGLContext()->FrontFace(
        opengl::SymbolConverter::toGLFrontFace(descriptor.frontFace));

    if (descriptor.depthBiasEnable) {
      glContext->gladGLContext()->Enable(GL_POLYGON_OFFSET_FILL);
    }

    glContext->gladGLContext()->PolygonOffset(
        descriptor.depthBiasSlopeFactor, descriptor.depthBiasConstantFactor);

    // TODO: OpenGL 4.6 or GL_ARB_polygon_offset_clamp
    // glContext->gladGLContext()->PolygonOffsetClamp(
    //     descriptor.depthBiasSlopeFactor, descriptor.depthBiasConstantFactor,
    //     descriptor.depthBiasClamp);

    glContext->gladGLContext()->LineWidth(descriptor.lineWidth);
    mRasterizationStateDirty = false;
  }

  // Depth Stencil State

  if (mDepthStencilStateDirty) {
    DepthStencilStateDescriptor descriptor;

    if (mState.depthStencilState) {
      descriptor = mState.depthStencilState->descriptor();
    } else {
      descriptor = DepthStencilStateDescriptor{};
    }

    if (descriptor.depthTestEnable) {
      glContext->gladGLContext()->Enable(GL_DEPTH_TEST);
    } else {
      glContext->gladGLContext()->Disable(GL_DEPTH_TEST);
    }

    glContext->gladGLContext()->DepthMask(
        descriptor.depthWriteEnable ? GL_TRUE : GL_FALSE);

    glContext->gladGLContext()->DepthFunc(
        opengl::SymbolConverter::toGLCompareFunc(descriptor.depthCompareOp));

    if (descriptor.stencilTestEnable) {
      glContext->gladGLContext()->Enable(GL_STENCIL_TEST);
    } else {
      glContext->gladGLContext()->Disable(GL_STENCIL_TEST);
    }

    glContext->gladGLContext()->StencilFuncSeparate(
        GL_FRONT,
        opengl::SymbolConverter::toGLCompareFunc(descriptor.front.compareOp),
        descriptor.front.reference, descriptor.front.compareMask);
    glContext->gladGLContext()->StencilOpSeparate(
        GL_FRONT,
        opengl::SymbolConverter::toGLStencilOp(descriptor.front.failOp),
        opengl::SymbolConverter::toGLStencilOp(descriptor.front.depthFailOp),
        opengl::SymbolConverter::toGLStencilOp(descriptor.front.passOp));
    glContext->gladGLContext()->StencilMaskSeparate(GL_FRONT,
                                                    descriptor.front.writeMask);
    glContext->gladGLContext()->StencilFuncSeparate(
        GL_BACK,
        opengl::SymbolConverter::toGLCompareFunc(descriptor.back.compareOp),
        descriptor.back.reference, descriptor.back.compareMask);
    glContext->gladGLContext()->StencilOpSeparate(
        GL_BACK, opengl::SymbolConverter::toGLStencilOp(descriptor.back.failOp),
        opengl::SymbolConverter::toGLStencilOp(descriptor.back.depthFailOp),
        opengl::SymbolConverter::toGLStencilOp(descriptor.back.passOp));
    glContext->gladGLContext()->StencilMaskSeparate(GL_BACK,
                                                    descriptor.back.writeMask);

    // TODO:
    // if (descriptor.depthBoundsTestEnable) {
    //   glContext->gladGLContext()->Enable(GL_DEPTH_BOUNDS_TEST_EXT);
    //   glContext->gladGLContext()->DepthBoundsEXT(descriptor.minDepthBounds,
    //                                              descriptor.maxDepthBounds);
    // } else {
    //   glContext->gladGLContext()->Disable(GL_DEPTH_BOUNDS_TEST_EXT);
    // }

    mDepthStencilStateDirty = false;
  }

  // Vertex Input State

  if (mVertexInputStateDirty) {
    if (mState.vertexInputState) {
      auto vertexArrayExp = mState.vertexInputState->glVertexArray(glContext);

      if (vertexArrayExp) {
        glContext->gladGLContext()->BindVertexArray(*vertexArrayExp);

        for (auto &&attribute :
             mState.vertexInputState->descriptor().attributes) {
          // glBindVertexBuffer emulation

          auto attribOpt =
              opengl::SymbolConverter::toGLVertexAttrib(attribute.format);

          if (!attribOpt) {
            // TODO: Report error
            continue;
          }

          auto attrib = *attribOpt;

          auto &&binding = mState.vertexBufferBinding[attribute.binding];

          if (!binding) {
            // TODO: Report error
            continue;
          }

          auto bindingDesc =
              mState.vertexInputState->getBindingDescriptor(attribute.binding);
          if (!bindingDesc) {
            // TODO: Report error
            continue;
          }

          glContext->gladGLContext()->BindBuffer(GL_ARRAY_BUFFER,
                                                 binding->buffer->glBuffer());
          glContext->gladGLContext()->VertexAttribPointer(
              attribute.location, std::get<0>(attrib), std::get<1>(attrib),
              std::get<2>(attrib), bindingDesc->stride,
              reinterpret_cast<const void *>(
                  static_cast<uintptr_t>(attribute.offset + binding->offset)));
          glContext->gladGLContext()->VertexAttribDivisor(
              attribute.location,
              (bindingDesc->inputRate == VertexInputRate::eVertex ? 0 : 1));
          glContext->gladGLContext()->BindBuffer(GL_ARRAY_BUFFER, 0);
        }
      }
    } else {
      glContext->gladGLContext()->BindVertexArray(0);
    }

    mVertexInputStateDirty = false;
  }

  // Uniform Buffer

  if (mUniformBufferDirty) {
    for (uint32_t i = 0; i < mState.uniformBufferBinding.size(); i++) {
      auto &&binding = mState.uniformBufferBinding[i];

      if (!binding) {
        continue;
      }

      glContext->gladGLContext()->BindBufferRange(
          GL_UNIFORM_BUFFER, i, binding->buffer->glBuffer(), binding->offset,
          binding->size);
    }

    mUniformBufferDirty = false;
  }

  // Texture

  if (mTextureBindingDirty) {
    for (uint32_t i = 0; i < mState.textureBinding.size(); i++) {
      auto &&binding = mState.textureBinding[i];
      glContext->gladGLContext()->ActiveTexture(GL_TEXTURE0 + i);

      if (!binding) {
        glContext->gladGLContext()->BindTexture(GL_TEXTURE_2D, 0);
        continue;
      }

      auto glTextureType = opengl::SymbolConverter::toGLTextureType(
          binding->descriptor().type, binding->descriptor().samples);

      if (!glTextureType) {
        // TODO: Report error
        continue;
      }

      glContext->gladGLContext()->ActiveTexture(GL_TEXTURE0 + i);
      glContext->gladGLContext()->BindTexture(*glTextureType,
                                              binding->glTexture());
    }

    mTextureBindingDirty = false;
  }

  if (mSamplerBindingDirty) {
    for (uint32_t i = 0; i < mState.samplerBinding.size(); i++) {
      auto &&binding = mState.samplerBinding[i];

      if (!binding) {
        glContext->gladGLContext()->BindSampler(i, 0);
        continue;
      }

      glContext->gladGLContext()->BindSampler(i, binding->glSampler());
    }

    mSamplerBindingDirty = false;
  }
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
