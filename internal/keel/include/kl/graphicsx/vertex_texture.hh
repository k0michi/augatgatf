#ifndef KL_GRAPHICSX_VERTEX_TEXTURE_HH
#define KL_GRAPHICSX_VERTEX_TEXTURE_HH

#include "kl/math/vector2.hh"
#include "kl/math/vector3.hh"

namespace kl::graphicsx {
struct VertexTexture {
  kl::math::Vector3 position;
  kl::math::Vector2 textureCoordinate;
};
} // namespace kl::graphicsx
#endif