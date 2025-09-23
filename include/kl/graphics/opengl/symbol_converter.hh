#ifndef KL_GRAPHICS_OPENGL_SYMBOL_CONVERTER_HH
#define KL_GRAPHICS_OPENGL_SYMBOL_CONVERTER_HH

#include <optional>
#include <tuple>
#include <utility>

#include <glad/gl.h>

#include "../format.hh"
#include "../texture_descriptor.hh"

namespace kl::graphics::opengl {
class SymbolConverter {
  SymbolConverter() = delete;

public:
  static constexpr std::optional<std::tuple<int32_t, int32_t, int32_t, int32_t>>
  toColorSizes(kl::graphics::Format format) {
    switch (format) {
    case kl::graphics::Format::eR4G4UnormPack8:
      return std::make_tuple(4, 4, 0, 0);
    case kl::graphics::Format::eR4G4B4A4UnormPack16:
      return std::make_tuple(4, 4, 4, 4);
    case kl::graphics::Format::eB4G4R4A4UnormPack16:
      return std::make_tuple(4, 4, 4, 4);
    case kl::graphics::Format::eR5G6B5UnormPack16:
      return std::make_tuple(5, 6, 5, 0);
    case kl::graphics::Format::eB5G6R5UnormPack16:
      return std::make_tuple(5, 6, 5, 0);
    case kl::graphics::Format::eR5G5B5A1UnormPack16:
      return std::make_tuple(5, 5, 5, 1);
    case kl::graphics::Format::eB5G5R5A1UnormPack16:
      return std::make_tuple(5, 5, 5, 1);
    case kl::graphics::Format::eA1R5G5B5UnormPack16:
      return std::make_tuple(5, 5, 5, 1);
    case kl::graphics::Format::eR8Unorm:
    case kl::graphics::Format::eR8Snorm:
    case kl::graphics::Format::eR8Uscaled:
    case kl::graphics::Format::eR8Sscaled:
    case kl::graphics::Format::eR8Uint:
    case kl::graphics::Format::eR8Sint:
    case kl::graphics::Format::eR8Srgb:
      return std::make_tuple(8, 0, 0, 0);
    case kl::graphics::Format::eR8G8Unorm:
    case kl::graphics::Format::eR8G8Snorm:
    case kl::graphics::Format::eR8G8Uscaled:
    case kl::graphics::Format::eR8G8Sscaled:
    case kl::graphics::Format::eR8G8Uint:
    case kl::graphics::Format::eR8G8Sint:
    case kl::graphics::Format::eR8G8Srgb:
      return std::make_tuple(8, 8, 0, 0);
    case kl::graphics::Format::eR8G8B8Unorm:
    case kl::graphics::Format::eR8G8B8Snorm:
    case kl::graphics::Format::eR8G8B8Uscaled:
    case kl::graphics::Format::eR8G8B8Sscaled:
    case kl::graphics::Format::eR8G8B8Uint:
    case kl::graphics::Format::eR8G8B8Sint:
    case kl::graphics::Format::eR8G8B8Srgb:
      return std::make_tuple(8, 8, 8, 0);
    case kl::graphics::Format::eB8G8R8Unorm:
    case kl::graphics::Format::eB8G8R8Snorm:
    case kl::graphics::Format::eB8G8R8Uscaled:
    case kl::graphics::Format::eB8G8R8Sscaled:
    case kl::graphics::Format::eB8G8R8Uint:
    case kl::graphics::Format::eB8G8R8Sint:
    case kl::graphics::Format::eB8G8R8Srgb:
      return std::make_tuple(8, 8, 8, 0);
    case kl::graphics::Format::eR8G8B8A8Unorm:
    case kl::graphics::Format::eR8G8B8A8Snorm:
    case kl::graphics::Format::eR8G8B8A8Uscaled:
    case kl::graphics::Format::eR8G8B8A8Sscaled:
    case kl::graphics::Format::eR8G8B8A8Uint:
    case kl::graphics::Format::eR8G8B8A8Sint:
    case kl::graphics::Format::eR8G8B8A8Srgb:
      return std::make_tuple(8, 8, 8, 8);
    case kl::graphics::Format::eB8G8R8A8Unorm:
    case kl::graphics::Format::eB8G8R8A8Snorm:
    case kl::graphics::Format::eB8G8R8A8Uscaled:
    case kl::graphics::Format::eB8G8R8A8Sscaled:
    case kl::graphics::Format::eB8G8R8A8Uint:
    case kl::graphics::Format::eB8G8R8A8Sint:
    case kl::graphics::Format::eB8G8R8A8Srgb:
      return std::make_tuple(8, 8, 8, 8);
    case kl::graphics::Format::eA8B8G8R8UnormPack32:
    case kl::graphics::Format::eA8B8G8R8SnormPack32:
    case kl::graphics::Format::eA8B8G8R8UscaledPack32:
    case kl::graphics::Format::eA8B8G8R8SscaledPack32:
    case kl::graphics::Format::eA8B8G8R8UintPack32:
    case kl::graphics::Format::eA8B8G8R8SintPack32:
    case kl::graphics::Format::eA8B8G8R8SrgbPack32:
      return std::make_tuple(8, 8, 8, 8);
    case kl::graphics::Format::eA2R10G10B10UnormPack32:
    case kl::graphics::Format::eA2R10G10B10SnormPack32:
    case kl::graphics::Format::eA2R10G10B10UscaledPack32:
    case kl::graphics::Format::eA2R10G10B10SscaledPack32:
    case kl::graphics::Format::eA2R10G10B10UintPack32:
    case kl::graphics::Format::eA2R10G10B10SintPack32:
      return std::make_tuple(10, 10, 10, 2);
    case kl::graphics::Format::eA2B10G10R10UnormPack32:
    case kl::graphics::Format::eA2B10G10R10SnormPack32:
    case kl::graphics::Format::eA2B10G10R10UscaledPack32:
    case kl::graphics::Format::eA2B10G10R10SscaledPack32:
    case kl::graphics::Format::eA2B10G10R10UintPack32:
    case kl::graphics::Format::eA2B10G10R10SintPack32:
      return std::make_tuple(10, 10, 10, 2);
    default:
      // TODO: Handle other formats.
      return std::nullopt;
    }
  };

  static constexpr std::optional<std::tuple<int32_t, int32_t>>
  toDepthStencilSizes(kl::graphics::Format format) {
    switch (format) {
    case kl::graphics::Format::eD16Unorm:
      return std::make_tuple(16, 0);
    case kl::graphics::Format::eX8D24UnormPack32:
      return std::make_tuple(24, 0);
    case kl::graphics::Format::eD32Sfloat:
      return std::make_tuple(32, 0);
    case kl::graphics::Format::eS8Uint:
      return std::make_tuple(0, 8);
    case kl::graphics::Format::eD16UnormS8Uint:
      return std::make_tuple(16, 8);
    case kl::graphics::Format::eD24UnormS8Uint:
      return std::make_tuple(24, 8);
    case kl::graphics::Format::eD32SfloatS8Uint:
      return std::make_tuple(32, 8);
    default:
      return std::nullopt;
    }
  }

  static constexpr std::expected<GLenum, std::runtime_error>
  toGLTextureType(kl::graphics::TextureType type, uint32_t samples) {
    switch (type) {
    case kl::graphics::TextureType::e1D:
      if (samples > 1) {
        return std::unexpected(
            std::runtime_error("1D textures cannot be multisampled"));
      }
      return GL_TEXTURE_1D;
    case kl::graphics::TextureType::e2D:
      return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    case kl::graphics::TextureType::e3D:
      return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_3D;
    };

    std::unreachable();
  }

  /**
   * @brief Returns internalformat, format, type.
   */
  static constexpr std::tuple<GLenum, GLenum, GLenum>
  toGLFormat(kl::graphics::Format format) {
    switch (format) {
    case kl::graphics::Format::eR8Unorm:
      return {GL_R8, GL_RED, GL_UNSIGNED_BYTE};
    case kl::graphics::Format::eR8G8Unorm:
      return {GL_RG8, GL_RG, GL_UNSIGNED_BYTE};
    case kl::graphics::Format::eR8G8B8Unorm:
      return {GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE};
    case kl::graphics::Format::eR8G8B8A8Unorm:
      return {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE};
    case kl::graphics::Format::eB8G8R8A8Unorm:
      return {GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE};
    case kl::graphics::Format::eD16Unorm:
      return {GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT};
    case kl::graphics::Format::eD32Sfloat:
      return {GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT};
    case kl::graphics::Format::eD24UnormS8Uint:
      return {GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8};
    case kl::graphics::Format::eD32SfloatS8Uint:
      return {GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL,
              GL_FLOAT_32_UNSIGNED_INT_24_8_REV};
    default:
      // TODO: Handle other formats.
      throw std::runtime_error("Unsupported format");
    }
  }
};
} // namespace kl::graphics::opengl
#endif // KL_GRAPHICS_OPENGL_SYMBOL_CONVERTER_HH
