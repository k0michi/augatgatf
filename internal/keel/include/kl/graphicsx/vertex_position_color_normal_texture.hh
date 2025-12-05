#ifndef KL_GRAPHICSX_VERTEX_POSITION_COLOR_NORMAL_TEXTURE_HH
#define KL_GRAPHICSX_VERTEX_POSITION_COLOR_NORMAL_TEXTURE_HH

#include "kl/math/vector2.hh"
#include "kl/math/vector3.hh"
#include "kl/math/vector4.hh"

namespace kl::graphicsx {
struct VertexPositionColorNormalTexture {
  kl::math::Vector3 position;
  kl::math::Vector4 color;
  kl::math::Vector3 normal;
  kl::math::Vector2 textureCoordinate;
};
} // namespace kl::graphicsx
#endif