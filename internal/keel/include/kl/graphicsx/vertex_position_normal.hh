#ifndef KL_GRAPHICSX_VERTEX_POSITION_NORMAL_HH
#define KL_GRAPHICSX_VERTEX_POSITION_NORMAL_HH

#include "kl/math/vector3.hh"

namespace kl::graphicsx {
struct VertexPositionNormal {
  kl::math::Vector3 position;
  kl::math::Vector3 normal;
};
} // namespace kl::graphicsx
#endif