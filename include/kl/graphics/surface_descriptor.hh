#ifndef KL_GRAPHICS_SURFACE_DESCRIPTOR_HH
#define KL_GRAPHICS_SURFACE_DESCRIPTOR_HH

#include "kl/platform/window.hh"

namespace kl::graphics {
struct SurfaceDescriptor final {
  std::shared_ptr<platform::Window> window;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_SURFACE_DESCRIPTOR_HH
