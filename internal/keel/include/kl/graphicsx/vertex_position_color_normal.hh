#ifndef KL_GRAPHICSX_VERTEX_POSITION_COLOR_NORMAL_HH
#define KL_GRAPHICSX_VERTEX_POSITION_COLOR_NORMAL_HH

#include "kl/math/vector3.hh"
#include "kl/math/vector4.hh"

namespace kl::graphicsx {
struct VertexPositionColorNormal {
  kl::math::Vector3 position;
  kl::math::Vector4 color;
  kl::math::Vector3 normal;
};
} // namespace kl::graphicsx
#endif