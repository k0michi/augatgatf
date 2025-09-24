#include "kl/graphics/sampler.hh"

#include "kl/graphics/device.hh"
#include "kl/graphics/opengl/symbol_converter.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<Sampler>, std::runtime_error>
Sampler::create(std::shared_ptr<Device> device,
                const SamplerDescriptor &descriptor) noexcept {
  auto sampler = std::shared_ptr<Sampler>(new Sampler(device));
  sampler->mDescriptor = descriptor;
  auto context = *(device->defaultContext());
  std::scoped_lock lock(*context);
  context->gladGLContext()->GenSamplers(1, &sampler->mSampler);

  if (sampler->mSampler == 0) {
    return std::unexpected(std::runtime_error("Failed to create sampler"));
  }

  context->gladGLContext()->SamplerParameteri(
      sampler->mSampler, GL_TEXTURE_MAG_FILTER,
      opengl::SymbolConverter::toGLFilter(descriptor.magFilter));
  context->gladGLContext()->SamplerParameteri(
      sampler->mSampler, GL_TEXTURE_MIN_FILTER,
      opengl::SymbolConverter::toGLFilter(descriptor.minFilter,
                                          descriptor.mipmapMode));
  context->gladGLContext()->SamplerParameteri(
      sampler->mSampler, GL_TEXTURE_WRAP_S,
      opengl::SymbolConverter::toGLSamplerWrap(descriptor.addressModeU));
  context->gladGLContext()->SamplerParameteri(
      sampler->mSampler, GL_TEXTURE_WRAP_T,
      opengl::SymbolConverter::toGLSamplerWrap(descriptor.addressModeV));
  context->gladGLContext()->SamplerParameteri(
      sampler->mSampler, GL_TEXTURE_WRAP_R,
      opengl::SymbolConverter::toGLSamplerWrap(descriptor.addressModeW));
  context->gladGLContext()->SamplerParameterf(
      sampler->mSampler, GL_TEXTURE_LOD_BIAS, descriptor.mipLodBias);

  // TODO: Anisotropy (OpenGL 4.6 or GL_EXT_texture_filter_anisotropic)

  if (descriptor.compareEnable) {
    context->gladGLContext()->SamplerParameteri(
        sampler->mSampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    context->gladGLContext()->SamplerParameteri(
        sampler->mSampler, GL_TEXTURE_COMPARE_FUNC,
        opengl::SymbolConverter::toGLCompareFunc(descriptor.compareOp));
  } else {
    context->gladGLContext()->SamplerParameteri(
        sampler->mSampler, GL_TEXTURE_COMPARE_MODE, GL_NONE);
  }

  context->gladGLContext()->SamplerParameterf(
      sampler->mSampler, GL_TEXTURE_MIN_LOD, descriptor.minLod);
  context->gladGLContext()->SamplerParameterf(
      sampler->mSampler, GL_TEXTURE_MAX_LOD, descriptor.maxLod);
  auto borderColor =
      opengl::SymbolConverter::toGLBorderColor(descriptor.borderColor);
  context->gladGLContext()->SamplerParameterfv(
      sampler->mSampler, GL_TEXTURE_BORDER_COLOR, &std::get<0>(borderColor));

  return sampler;
}

Sampler::Sampler(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics
