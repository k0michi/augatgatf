#ifndef KL_GRAPHICS_OPENGL_SURFACE_CONFIG_HH
#define KL_GRAPHICS_OPENGL_SURFACE_CONFIG_HH

#include <cstdint>

namespace kl::graphics::opengl {
struct SurfaceConfig final {
  int32_t redSize = 8;
  int32_t greenSize = 8;
  int32_t blueSize = 8;
  int32_t alphaSize = 8;
  int32_t depthSize = 24;
  int32_t stencilSize = 8;
  int32_t doubleBuffer = 1;
  int32_t multiSampleBuffers = 0;
  int32_t multiSampleSamples = 0;
};

constexpr bool operator==(const SurfaceConfig &lhs,
                          const SurfaceConfig &rhs) noexcept {
  return lhs.redSize == rhs.redSize && lhs.greenSize == rhs.greenSize &&
         lhs.blueSize == rhs.blueSize && lhs.alphaSize == rhs.alphaSize &&
         lhs.depthSize == rhs.depthSize && lhs.stencilSize == rhs.stencilSize &&
         lhs.doubleBuffer == rhs.doubleBuffer &&
         lhs.multiSampleBuffers == rhs.multiSampleBuffers &&
         lhs.multiSampleSamples == rhs.multiSampleSamples;
}

constexpr bool operator!=(const SurfaceConfig &lhs,
                          const SurfaceConfig &rhs) noexcept {
  return !(lhs == rhs);
}
} // namespace kl::graphics::opengl
#endif // KL_GRAPHICS_OPENGL_SURFACE_CONFIG_HH
