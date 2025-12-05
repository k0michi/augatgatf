#ifndef KL_GRAPHICSX_VERTEX_POSITION_COLOR_HH
#define KL_GRAPHICSX_VERTEX_POSITION_COLOR_HH

#include "kl/math/vector3.hh"
#include "kl/math/vector4.hh"

namespace kl::graphicsx {
struct VertexPositionColor {
  kl::math::Vector3 position;
  kl::math::Vector4 color;
};
} // namespace kl::graphicsx
#endif