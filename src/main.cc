#include <SDL3/SDL.h>
#include <iostream>

#include "kl/graphics/instance.hh"
#include "kl/platform/instance.hh"
#include "kl/platform/task.hh"
#include "kl/platform/window.hh"

kl::platform::Task<void> main_async() {
  auto instanceResult =
      kl::platform::Instance::create(kl::InstanceDescriptor{});
  if (!instanceResult) {
    std::cerr << "Failed to create Instance: " << instanceResult.error().what()
              << std::endl;
    co_return;
  }

  auto instance = instanceResult.value();
  auto windowResult = instance->createWindow(
      kl::platform::WindowDescriptor{"My Window", 800, 600});
  if (!windowResult) {
    std::cerr << "Failed to create Window: " << windowResult.error().what()
              << std::endl;
    co_return;
  }

  auto graphicsInstance =
      kl::graphics::Instance::create(kl::graphics::InstanceDescriptor{})
          .value();
  kl::graphics::SurfaceDescriptor surfaceDesc;
  surfaceDesc.window = windowResult.value();
  auto surface = graphicsInstance->createSurface(surfaceDesc);
  auto device =
      graphicsInstance->createDevice(kl::graphics::DeviceDescriptor{});

  auto rasterizationState = device.value()->createRasterizationState({});

  auto window = windowResult.value();
  int32_t count = 0;

  while (!instance->shouldQuit()) {
    auto elapsed = co_await instance->waitFrame();
    instance->pollEvents();
    // std::cout << elapsed << std::endl;
    count++;
  }
}

int main() {
  main_async();
  return 0;
}
