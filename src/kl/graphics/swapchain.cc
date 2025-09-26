#include "kl/graphics/swapchain.hh"

#include "kl/graphics/device.hh"
#include "kl/graphics/opengl/surface_config.hh"
#include "kl/graphics/opengl/symbol_converter.hh"

namespace kl::graphics {
void Swapchain::present(int32_t interval) noexcept {
  auto device = this->device().lock();

  if (!device) {
    return;
  }

  auto context = device->getContextForWindow(mDescriptor.window);

  if (!context) {
    return;
  }

  std::scoped_lock lock(**context);
  // Prevent "Cannot set timing mode for main loop since a main loop does not
  // exist! Call emscripten_set_main_loop first to set one up."
#ifndef __EMSCRIPTEN__
  SDL_GL_SetSwapInterval(interval);
#endif
  SDL_GL_SwapWindow(mDescriptor.window->sdlWindow());
}

std::expected<std::shared_ptr<Swapchain>, std::runtime_error>
Swapchain::create(std::shared_ptr<Device> device,
                  const SwapchainDescriptor &descriptor) noexcept {
  auto swapchain = std::shared_ptr<Swapchain>(new Swapchain(device));
  swapchain->mDescriptor = descriptor;
  auto framebufferResult =
      Framebuffer::createDefault(device, descriptor.window);

  if (!framebufferResult) {
    // createDefault never fails.
    return std::unexpected(framebufferResult.error());
  }

  swapchain->mFramebuffer = framebufferResult.value();

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

  if (descriptor.window->glSurfaceConfig() != glSurfaceConfig) {
    descriptor.window->setGLSurfaceConfig(glSurfaceConfig);

    if (!descriptor.window->recreate()) {
      return std::unexpected(
          std::runtime_error("Failed to recreate window for swapchain"));
    }
  }

#ifdef __EMSCRIPTEN__
  device->getOrCreateContextForWindow(descriptor.window);
#endif

  return swapchain;
}

Swapchain::Swapchain(std::shared_ptr<Device> device) noexcept
    : DeviceChild(std::move(device)) {}
} // namespace kl::graphics
