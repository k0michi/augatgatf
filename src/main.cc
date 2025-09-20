#include <SDL3/SDL.h>
#include <iostream>

#include "kl/platform/instance.hh"
#include "kl/platform/window.hh"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(
      [](void *arg) {
        auto instance = static_cast<kl::platform::Instance *>(arg);
        if (instance->shouldQuit()) {
          emscripten_cancel_main_loop();
          return;
        }
        instance->pollEvents();
      },
      instance.get(), 0, true);
#else
  while (!instance->shouldQuit()) {
    instance->pollEvents();
  }
#endif

  return 0;
}
