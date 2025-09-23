#ifndef KL_GRAPHICS_SWAPCHAIN_DESCRIPTOR_HH
#define KL_GRAPHICS_SWAPCHAIN_DESCRIPTOR_HH

#include "format.hh"
#include "kl/platform/window.hh"

namespace kl::graphics {
struct SwapchainDescriptor final {
  std::shared_ptr<platform::Window> window;
  Format colorFormat = Format::eR8G8B8A8Unorm;
  Format depthStencilFormat = Format::eD16UnormS8Uint;
  bool doubleBuffered = true;
};
} // namespace kl::graphics
#endif // KL_GRAPHICS_SWAPCHAIN_DESCRIPTOR_HH
