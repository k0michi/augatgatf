#include "kl/graphics/texture.hh"

#include "kl/graphics/device.hh"
#include "kl/graphics/opengl/symbol_converter.hh"

#include <algorithm>
#include <cmath>

namespace kl::graphics {
std::expected<std::shared_ptr<Texture>, std::runtime_error>
Texture::create(std::shared_ptr<Device> device,
                const TextureDescriptor &descriptor) noexcept {
  auto texture = std::shared_ptr<Texture>(new Texture(device));
  texture->mDescriptor = descriptor;

  auto context = *(device->defaultContext());
  std::scoped_lock lock(*context);
  context->gladGLContext()->GenTextures(1, &texture->mTexture);

  if (texture->mTexture == 0) {
    return std::unexpected(
        std::runtime_error("Failed to create OpenGL texture"));
  }

#if __EMSCRIPTEN__
  if (texture->mDescriptor.type == TextureType::e1D) {
    return std::unexpected(std::runtime_error(
        "1D textures are not supported in WebGL 2 / OpenGL ES 3.0"));
  }
#endif

  auto type =
      opengl::SymbolConverter::toGLTextureType(texture->mDescriptor.type);
  context->gladGLContext()->BindTexture(type, texture->mTexture);
  auto format =
      opengl::SymbolConverter::toGLFormat(texture->mDescriptor.format);

  if (!texture->mDescriptor.mipLevels.has_value()) {
    texture->mDescriptor.mipLevels =
        std::floor(std::log2(std::max({texture->mDescriptor.extent.width,
                                       texture->mDescriptor.extent.height,
                                       texture->mDescriptor.extent.depth}))) +
        1;
  }

  // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml

  uint32_t width = texture->mDescriptor.extent.width;
  uint32_t height = texture->mDescriptor.extent.height;
  uint32_t depth = texture->mDescriptor.extent.depth;

  if (type == GL_TEXTURE_1D) {
    for (uint32_t i = 0; i < texture->mDescriptor.mipLevels.value(); ++i) {
      context->gladGLContext()->TexImage1D(
          GL_TEXTURE_1D, i, std::get<0>(format), width, 0, std::get<1>(format),
          std::get<2>(format), nullptr);
      width = std::max(1u, width / 2);
    }
  } else if (type == GL_TEXTURE_2D) {
    for (uint32_t i = 0; i < texture->mDescriptor.mipLevels.value(); ++i) {
      context->gladGLContext()->TexImage2D(
          GL_TEXTURE_2D, i, std::get<0>(format), width, height, 0,
          std::get<1>(format), std::get<2>(format), nullptr);
      width = std::max(1u, width / 2);
      height = std::max(1u, height / 2);
    }
  } else if (type == GL_TEXTURE_3D) {
    for (uint32_t i = 0; i < texture->mDescriptor.mipLevels.value(); ++i) {
      context->gladGLContext()->TexImage3D(
          GL_TEXTURE_3D, i, std::get<0>(format), width, height, depth, 0,
          std::get<1>(format), std::get<2>(format), nullptr);
      width = std::max(1u, width / 2);
      height = std::max(1u, height / 2);
      depth = std::max(1u, depth / 2);
    }
  }

  context->gladGLContext()->TexParameteri(type, GL_TEXTURE_BASE_LEVEL, 0);
  context->gladGLContext()->TexParameteri(
      type, GL_TEXTURE_MAX_LEVEL, texture->mDescriptor.mipLevels.value() - 1);
  context->gladGLContext()->BindTexture(type, 0);

  return texture;
}

Texture::Texture(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics
