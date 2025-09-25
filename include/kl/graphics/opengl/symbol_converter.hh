#ifndef KL_GRAPHICS_OPENGL_SYMBOL_CONVERTER_HH
#define KL_GRAPHICS_OPENGL_SYMBOL_CONVERTER_HH

#include <expected>
#include <optional>
#include <tuple>
#include <utility>

#include <glad/gl.h>

#include "../color_blend_state_descriptor.hh"
#include "../format.hh"
#include "../primitive_topology.hh"
#include "../rasterization_state_descriptor.hh"
#include "../sampler_descriptor.hh"
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

  static std::expected<GLenum, std::runtime_error>
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

  static constexpr GLenum toGLCompareFunc(kl::graphics::CompareOp op) {
    switch (op) {
    case kl::graphics::CompareOp::eNever:
      return GL_NEVER;
    case kl::graphics::CompareOp::eLess:
      return GL_LESS;
    case kl::graphics::CompareOp::eEqual:
      return GL_EQUAL;
    case kl::graphics::CompareOp::eLessOrEqual:
      return GL_LEQUAL;
    case kl::graphics::CompareOp::eGreater:
      return GL_GREATER;
    case kl::graphics::CompareOp::eNotEqual:
      return GL_NOTEQUAL;
    case kl::graphics::CompareOp::eGreaterOrEqual:
      return GL_GEQUAL;
    case kl::graphics::CompareOp::eAlways:
      return GL_ALWAYS;
    }

    std::unreachable();
  }

  static constexpr GLenum
  toGLSamplerWrap(kl::graphics::SamplerAddressMode mode) {
    switch (mode) {
    case kl::graphics::SamplerAddressMode::eRepeat:
      return GL_REPEAT;
    case kl::graphics::SamplerAddressMode::eMirroredRepeat:
      return GL_MIRRORED_REPEAT;
    case kl::graphics::SamplerAddressMode::eClampToEdge:
      return GL_CLAMP_TO_EDGE;
    case kl::graphics::SamplerAddressMode::eClampToBorder:
      return GL_CLAMP_TO_BORDER;
      // OpenGL 4.4 or GL_ARB_texture_mirror_clamp_to_edge
    case kl::graphics::SamplerAddressMode::eMirrorClampToEdge:
      return GL_MIRROR_CLAMP_TO_EDGE;
    }

    std::unreachable();
  }

  static constexpr GLenum toGLFilter(kl::graphics::Filter filter,
                                     kl::graphics::SamplerMipmapMode mipmap) {
    switch (filter) {
    case kl::graphics::Filter::eNearest:
      switch (mipmap) {
      case kl::graphics::SamplerMipmapMode::eNearest:
        return GL_NEAREST_MIPMAP_NEAREST;
      case kl::graphics::SamplerMipmapMode::eLinear:
        return GL_NEAREST_MIPMAP_LINEAR;
      }
      break;
    case kl::graphics::Filter::eLinear:
      switch (mipmap) {
      case kl::graphics::SamplerMipmapMode::eNearest:
        return GL_LINEAR_MIPMAP_NEAREST;
      case kl::graphics::SamplerMipmapMode::eLinear:
        return GL_LINEAR_MIPMAP_LINEAR;
      }
      break;
    }

    std::unreachable();
  }

  static constexpr GLenum toGLFilter(kl::graphics::Filter filter) {
    switch (filter) {
    case kl::graphics::Filter::eNearest:
      return GL_NEAREST;
    case kl::graphics::Filter::eLinear:
      return GL_LINEAR;
    }

    std::unreachable();
  }

  static constexpr std::tuple<GLfloat, GLfloat, GLfloat, GLfloat>
  toGLBorderColor(kl::graphics::BorderColor color) {
    switch (color) {
    case kl::graphics::BorderColor::eTransparentBlack:
      return {0.0f, 0.0f, 0.0f, 0.0f};
    case kl::graphics::BorderColor::eOpaqueBlack:
      return {0.0f, 0.0f, 0.0f, 1.0f};
    case kl::graphics::BorderColor::eOpaqueWhite:
      return {1.0f, 1.0f, 1.0f, 1.0f};
    }

    std::unreachable();
  }

  static constexpr GLenum toGLBlendFunc(kl::graphics::BlendFactor factor) {
    switch (factor) {
    case kl::graphics::BlendFactor::eZero:
      return GL_ZERO;
    case kl::graphics::BlendFactor::eOne:
      return GL_ONE;
    case kl::graphics::BlendFactor::eSrcColor:
      return GL_SRC_COLOR;
    case kl::graphics::BlendFactor::eOneMinusSrcColor:
      return GL_ONE_MINUS_SRC_COLOR;
    case kl::graphics::BlendFactor::eDstColor:
      return GL_DST_COLOR;
    case kl::graphics::BlendFactor::eOneMinusDstColor:
      return GL_ONE_MINUS_DST_COLOR;
    case kl::graphics::BlendFactor::eSrcAlpha:
      return GL_SRC_ALPHA;
    case kl::graphics::BlendFactor::eOneMinusSrcAlpha:
      return GL_ONE_MINUS_SRC_ALPHA;
    case kl::graphics::BlendFactor::eDstAlpha:
      return GL_DST_ALPHA;
    case kl::graphics::BlendFactor::eOneMinusDstAlpha:
      return GL_ONE_MINUS_DST_ALPHA;
    case kl::graphics::BlendFactor::eConstantColor:
      return GL_CONSTANT_COLOR;
    case kl::graphics::BlendFactor::eOneMinusConstantColor:
      return GL_ONE_MINUS_CONSTANT_COLOR;
    case kl::graphics::BlendFactor::eConstantAlpha:
      return GL_CONSTANT_ALPHA;
    case kl::graphics::BlendFactor::eOneMinusConstantAlpha:
      return GL_ONE_MINUS_CONSTANT_ALPHA;
    case kl::graphics::BlendFactor::eSrcAlphaSaturate:
      return GL_SRC_ALPHA_SATURATE;
    case kl::graphics::BlendFactor::eSrc1Color:
      return GL_SRC1_COLOR;
    case kl::graphics::BlendFactor::eOneMinusSrc1Color:
      return GL_ONE_MINUS_SRC1_COLOR;
    case kl::graphics::BlendFactor::eSrc1Alpha:
      return GL_SRC1_ALPHA;
    case kl::graphics::BlendFactor::eOneMinusSrc1Alpha:
      return GL_ONE_MINUS_SRC1_ALPHA;
    }

    std::unreachable();
  }

  static constexpr GLenum toGLBlendEquation(kl::graphics::BlendOp op) {
    switch (op) {
    case kl::graphics::BlendOp::eAdd:
      return GL_FUNC_ADD;
    case kl::graphics::BlendOp::eSubtract:
      return GL_FUNC_SUBTRACT;
    case kl::graphics::BlendOp::eReverseSubtract:
      return GL_FUNC_REVERSE_SUBTRACT;
    case kl::graphics::BlendOp::eMin:
      return GL_MIN;
    case kl::graphics::BlendOp::eMax:
      return GL_MAX;
    }

    std::unreachable();
  }

  static constexpr std::tuple<GLboolean, GLboolean, GLboolean, GLboolean>
  toGLColorMask(kl::graphics::ColorComponent component) {
    return {
        static_cast<GLboolean>((component & kl::graphics::ColorComponent::eR) !=
                               kl::graphics::ColorComponent(0)),
        static_cast<GLboolean>((component & kl::graphics::ColorComponent::eG) !=
                               kl::graphics::ColorComponent(0)),
        static_cast<GLboolean>((component & kl::graphics::ColorComponent::eB) !=
                               kl::graphics::ColorComponent(0)),
        static_cast<GLboolean>((component & kl::graphics::ColorComponent::eA) !=
                               kl::graphics::ColorComponent(0))};
  }

  static constexpr GLenum toGLPolygonMode(kl::graphics::PolygonMode mode) {
    switch (mode) {
    case kl::graphics::PolygonMode::eFill:
      return GL_FILL;
    case kl::graphics::PolygonMode::eLine:
      return GL_LINE;
    case kl::graphics::PolygonMode::ePoint:
      return GL_POINT;
    }
  }

  static constexpr std::optional<GLenum>
  toGLCullMode(kl::graphics::CullMode mode) {
    switch (mode) {
    case kl::graphics::CullMode::eNone:
      return std::nullopt;
    case kl::graphics::CullMode::eFront:
      return GL_FRONT;
    case kl::graphics::CullMode::eBack:
      return GL_BACK;
    case kl::graphics::CullMode::eFrontAndBack:
      return GL_FRONT_AND_BACK;
    }
  }

  static constexpr GLenum toGLFrontFace(kl::graphics::FrontFace face) {
    switch (face) {
    case kl::graphics::FrontFace::eCounterClockwise:
      return GL_CCW;
    case kl::graphics::FrontFace::eClockwise:
      return GL_CW;
    }
  }

  // (size, type, normalized)
  static constexpr std::optional<std::tuple<GLint, GLenum, GLboolean>>
  toGLVertexAttrib(kl::graphics::Format format) {
    switch (format) {
    case kl::graphics::Format::eR8Unorm:
      return std::make_tuple(1, GL_UNSIGNED_BYTE, GL_TRUE);
    case kl::graphics::Format::eR8Snorm:
      return std::make_tuple(1, GL_BYTE, GL_TRUE);
    case kl::graphics::Format::eR8Uscaled:
      return std::make_tuple(1, GL_UNSIGNED_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8Sscaled:
      return std::make_tuple(1, GL_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8Uint:
      return std::make_tuple(1, GL_UNSIGNED_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8Sint:
      return std::make_tuple(1, GL_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8Unorm:
      return std::make_tuple(2, GL_UNSIGNED_BYTE, GL_TRUE);
    case kl::graphics::Format::eR8G8Snorm:
      return std::make_tuple(2, GL_BYTE, GL_TRUE);
    case kl::graphics::Format::eR8G8Uscaled:
      return std::make_tuple(2, GL_UNSIGNED_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8Sscaled:
      return std::make_tuple(2, GL_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8Uint:
      return std::make_tuple(2, GL_UNSIGNED_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8Sint:
      return std::make_tuple(2, GL_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8B8Unorm:
      return std::make_tuple(3, GL_UNSIGNED_BYTE, GL_TRUE);
    case kl::graphics::Format::eR8G8B8Snorm:
      return std::make_tuple(3, GL_BYTE, GL_TRUE);
    case kl::graphics::Format::eR8G8B8Uscaled:
      return std::make_tuple(3, GL_UNSIGNED_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8B8Sscaled:
      return std::make_tuple(3, GL_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8B8Uint:
      return std::make_tuple(3, GL_UNSIGNED_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8B8Sint:
      return std::make_tuple(3, GL_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8B8A8Unorm:
      return std::make_tuple(4, GL_UNSIGNED_BYTE, GL_TRUE);
    case kl::graphics::Format::eR8G8B8A8Snorm:
      return std::make_tuple(4, GL_BYTE, GL_TRUE);
    case kl::graphics::Format::eR8G8B8A8Uscaled:
      return std::make_tuple(4, GL_UNSIGNED_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8B8A8Sscaled:
      return std::make_tuple(4, GL_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8B8A8Uint:
      return std::make_tuple(4, GL_UNSIGNED_BYTE, GL_FALSE);
    case kl::graphics::Format::eR8G8B8A8Sint:
      return std::make_tuple(4, GL_BYTE, GL_FALSE);
    case kl::graphics::Format::eR16Unorm:
      return std::make_tuple(1, GL_UNSIGNED_SHORT, GL_TRUE);
    case kl::graphics::Format::eR16Snorm:
      return std::make_tuple(1, GL_SHORT, GL_TRUE);
    case kl::graphics::Format::eR16Uscaled:
      return std::make_tuple(1, GL_UNSIGNED_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16Sscaled:
      return std::make_tuple(1, GL_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16Uint:
      return std::make_tuple(1, GL_UNSIGNED_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16Sint:
      return std::make_tuple(1, GL_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16Sfloat:
      return std::make_tuple(1, GL_HALF_FLOAT, GL_FALSE);
    case kl::graphics::Format::eR16G16Unorm:
      return std::make_tuple(2, GL_UNSIGNED_SHORT, GL_TRUE);
    case kl::graphics::Format::eR16G16Snorm:
      return std::make_tuple(2, GL_SHORT, GL_TRUE);
    case kl::graphics::Format::eR16G16Uscaled:
      return std::make_tuple(2, GL_UNSIGNED_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16Sscaled:
      return std::make_tuple(2, GL_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16Uint:
      return std::make_tuple(2, GL_UNSIGNED_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16Sint:
      return std::make_tuple(2, GL_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16Sfloat:
      return std::make_tuple(2, GL_HALF_FLOAT, GL_FALSE);
    case kl::graphics::Format::eR16G16B16Unorm:
      return std::make_tuple(3, GL_UNSIGNED_SHORT, GL_TRUE);
    case kl::graphics::Format::eR16G16B16Snorm:
      return std::make_tuple(3, GL_SHORT, GL_TRUE);
    case kl::graphics::Format::eR16G16B16Uscaled:
      return std::make_tuple(3, GL_UNSIGNED_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16B16Sscaled:
      return std::make_tuple(3, GL_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16B16Uint:
      return std::make_tuple(3, GL_UNSIGNED_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16B16Sint:
      return std::make_tuple(3, GL_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16B16Sfloat:
      return std::make_tuple(3, GL_HALF_FLOAT, GL_FALSE);
    case kl::graphics::Format::eR16G16B16A16Unorm:
      return std::make_tuple(4, GL_UNSIGNED_SHORT, GL_TRUE);
    case kl::graphics::Format::eR16G16B16A16Snorm:
      return std::make_tuple(4, GL_SHORT, GL_TRUE);
    case kl::graphics::Format::eR16G16B16A16Uscaled:
      return std::make_tuple(4, GL_UNSIGNED_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16B16A16Sscaled:
      return std::make_tuple(4, GL_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16B16A16Uint:
      return std::make_tuple(4, GL_UNSIGNED_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16B16A16Sint:
      return std::make_tuple(4, GL_SHORT, GL_FALSE);
    case kl::graphics::Format::eR16G16B16A16Sfloat:
      return std::make_tuple(4, GL_HALF_FLOAT, GL_FALSE);
    case kl::graphics::Format::eR32Uint:
      return std::make_tuple(1, GL_UNSIGNED_INT, GL_FALSE);
    case kl::graphics::Format::eR32Sint:
      return std::make_tuple(1, GL_INT, GL_FALSE);
    case kl::graphics::Format::eR32Sfloat:
      return std::make_tuple(1, GL_FLOAT, GL_FALSE);
    case kl::graphics::Format::eR32G32Uint:
      return std::make_tuple(2, GL_UNSIGNED_INT, GL_FALSE);
    case kl::graphics::Format::eR32G32Sint:
      return std::make_tuple(2, GL_INT, GL_FALSE);
    case kl::graphics::Format::eR32G32Sfloat:
      return std::make_tuple(2, GL_FLOAT, GL_FALSE);
    case kl::graphics::Format::eR32G32B32Uint:
      return std::make_tuple(3, GL_UNSIGNED_INT, GL_FALSE);
    case kl::graphics::Format::eR32G32B32Sint:
      return std::make_tuple(3, GL_INT, GL_FALSE);
    case kl::graphics::Format::eR32G32B32Sfloat:
      return std::make_tuple(3, GL_FLOAT, GL_FALSE);
    case kl::graphics::Format::eR32G32B32A32Uint:
      return std::make_tuple(4, GL_UNSIGNED_INT, GL_FALSE);
    case kl::graphics::Format::eR32G32B32A32Sint:
      return std::make_tuple(4, GL_INT, GL_FALSE);
    case kl::graphics::Format::eR32G32B32A32Sfloat:
      return std::make_tuple(4, GL_FLOAT, GL_FALSE);
    default:
      return std::nullopt;
    }
  }

  constexpr static GLenum toGLStencilOp(kl::graphics::StencilOp op) {
    switch (op) {
    case kl::graphics::StencilOp::eKeep:
      return GL_KEEP;
    case kl::graphics::StencilOp::eZero:
      return GL_ZERO;
    case kl::graphics::StencilOp::eReplace:
      return GL_REPLACE;
    case kl::graphics::StencilOp::eIncrementAndClamp:
      return GL_INCR;
    case kl::graphics::StencilOp::eDecrementAndClamp:
      return GL_DECR;
    case kl::graphics::StencilOp::eInvert:
      return GL_INVERT;
    case kl::graphics::StencilOp::eIncrementAndWrap:
      return GL_INCR_WRAP;
    case kl::graphics::StencilOp::eDecrementAndWrap:
      return GL_DECR_WRAP;
    }

    std::unreachable();
  }

  constexpr static GLenum
  toGLDrawMode(kl::graphics::PrimitiveTopology topology) {
    switch (topology) {
    case kl::graphics::PrimitiveTopology::ePointList:
      return GL_POINTS;
    case kl::graphics::PrimitiveTopology::eLineList:
      return GL_LINES;
    case kl::graphics::PrimitiveTopology::eLineStrip:
      return GL_LINE_STRIP;
    case kl::graphics::PrimitiveTopology::eTriangleList:
      return GL_TRIANGLES;
    case kl::graphics::PrimitiveTopology::eTriangleStrip:
      return GL_TRIANGLE_STRIP;
    case kl::graphics::PrimitiveTopology::eTriangleFan:
      return GL_TRIANGLE_FAN;
    case kl::graphics::PrimitiveTopology::eLineListWithAdjacency:
      return GL_LINES_ADJACENCY;
    case kl::graphics::PrimitiveTopology::eLineStripWithAdjacency:
      return GL_LINE_STRIP_ADJACENCY;
    case kl::graphics::PrimitiveTopology::eTriangleListWithAdjacency:
      return GL_TRIANGLES_ADJACENCY;
    case kl::graphics::PrimitiveTopology::eTriangleStripWithAdjacency:
      return GL_TRIANGLE_STRIP_ADJACENCY;
    case kl::graphics::PrimitiveTopology::ePatchList:
      return GL_PATCHES;
    }

    std::unreachable();
  }
};
} // namespace kl::graphics::opengl
#endif // KL_GRAPHICS_OPENGL_SYMBOL_CONVERTER_HH
