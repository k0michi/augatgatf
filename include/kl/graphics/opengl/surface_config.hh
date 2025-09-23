#ifndef KL_GRAPHICS_OPENGL_SURFACE_CONFIG_HH
#define KL_GRAPHICS_OPENGL_SURFACE_CONFIG_HH

#include <cstdint>

namespace kl::graphics::opengl {
struct SurfaceConfig final {
  int32_t redSize = 8;
  int32_t greenSize = 8;
  int32_t blueSize = 8;
  int32_t alphaSize = 8;
  int32_t depthSize = 16;
  int32_t stencilSize = 8;
  int32_t doubleBuffer = 1;
  int32_t multiSampleBuffers = 0;
  int32_t multiSampleSamples = 0;
};
} // namespace kl::graphics::opengl
#endif // KL_GRAPHICS_OPENGL_SURFACE_CONFIG_HH
