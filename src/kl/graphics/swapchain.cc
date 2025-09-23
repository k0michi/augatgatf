#include "kl/graphics/swapchain.hh"

#include "kl/graphics/opengl/surface_config.hh"
#include "kl/graphics/opengl/symbol_converter.hh"

namespace kl::graphics {
std::expected<std::shared_ptr<Swapchain>, std::runtime_error>
Swapchain::create(std::shared_ptr<Device> device,
                  const SwapchainDescriptor &descriptor) noexcept {
  auto swapchain = std::shared_ptr<Swapchain>(new Swapchain(device));
  swapchain->mDescriptor = descriptor;

  // SDL has no way to change GL attributes afterwards, so we recreate the
  // window

  kl::graphics::opengl::SurfaceConfig glSurfaceConfig;
  auto colorSizes = kl::graphics::opengl::SymbolConverter::toColorSizes(
      descriptor.colorFormat);

  if (colorSizes) {
    std::tie(glSurfaceConfig.redSize, glSurfaceConfig.greenSize,
             glSurfaceConfig.blueSize, glSurfaceConfig.alphaSize) =
        colorSizes.value();
  }

  auto depthStencilSizes =
      kl::graphics::opengl::SymbolConverter::toDepthStencilSizes(
          descriptor.depthStencilFormat);

  if (depthStencilSizes) {
    std::tie(glSurfaceConfig.depthSize, glSurfaceConfig.stencilSize) =
        depthStencilSizes.value();
  }

  glSurfaceConfig.doubleBuffer = descriptor.doubleBuffered ? 1 : 0;
  descriptor.window->setGLSurfaceConfig(glSurfaceConfig);
  descriptor.window->recreate();
  return swapchain;
}

Swapchain::Swapchain(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics
