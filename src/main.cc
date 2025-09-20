#include <SDL3/SDL.h>
#include <iostream>

#include "kl/platform/instance.hh"
#include "kl/platform/window.hh"

int main() {
  std::cout << "Hello, World!" << std::endl;
  auto instanceResult =
      kl::platform::Instance::create(kl::InstanceDescriptor{});
  if (!instanceResult) {
    std::cerr << "Failed to create Instance: " << instanceResult.error().what()
              << std::endl;
    return 1;
  }

  auto instance = instanceResult.value();
  auto windowResult = instance->createWindow(
      kl::platform::WindowDescriptor{"My Window", 800, 600});
  if (!windowResult) {
    std::cerr << "Failed to create Window: " << windowResult.error().what()
              << std::endl;
    return 1;
  }

  auto window = windowResult.value();

  while (!instance->shouldQuit()) {
    instance->pollEvents();
  }

  return 0;
}
