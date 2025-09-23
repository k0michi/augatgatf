#include <iostream>

#include "kl/graphics/instance.hh"
#include "kl/math/matrix2x2.hh"
#include "kl/math/matrix3x3.hh"
#include "kl/math/vector2.hh"
#include "kl/math/vector3.hh"
#include "kl/math/vector4.hh"
#include "kl/platform/instance.hh"
#include "kl/platform/task.hh"
#include "kl/platform/window.hh"

#include <fstream>
#include <spirv_glsl.hpp>

kl::platform::Task<void> main_async() {
  kl::math::Vector2 a{3.0f, 4.0f};
  kl::math::Vector2 b{1.0f, 2.0f};
  kl::math::DVector2 d = a;
  long long x = 1;
  char i = x;
  kl::math::Vector2 c = d;
  std::cout << c << std::endl;
  kl::math::Matrix2x2 m{1.0f, 2.0f, 3.0f, 4.0f};
  auto inv = m.inverse();
  if (inv) {
    std::cout << *inv << std::endl;
  } else {
    std::cout << "Matrix is not invertible" << std::endl;
  }

  auto instanceResult =
      kl::platform::Instance::create(kl::platform::InstanceDescriptor{});
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
  auto device =
      graphicsInstance->createDevice(kl::graphics::DeviceDescriptor{});

  auto rasterizationState = device.value()->createRasterizationState({});

  {
    auto file = "shaders/test.vert.spv";
    std::ifstream fs(file, std::ios::binary | std::ios::ate);
    if (!fs.is_open()) {
      std::cerr << "Failed to open file: " << file << std::endl;
      co_return;
    }
    auto size = fs.tellg();
    fs.seekg(0, std::ios::beg);
    std::vector<std::byte> spirv_binary(size / sizeof(std::byte));
    fs.read(reinterpret_cast<char *>(spirv_binary.data()), size);
    fs.close();
    auto shader = device.value()->createShader({
        .stage = kl::graphics::ShaderStage::eVertex,
        .code = spirv_binary,
        .entryPoint = "main",
    });

    if (!shader) {
      std::cerr << "Failed to create shader: " << shader.error().what()
                << std::endl;
      co_return;
    }

    *shader;

    auto program = device.value()->createProgram({
        .shaders =
            {
                device.value()
                    ->createShader({
                        .stage = kl::graphics::ShaderStage::eVertex,
                        .code = spirv_binary,
                        .entryPoint = "main",
                    })
                    .value(),
            },
    });

    if (!program) {
      std::cerr << "Failed to create program: " << program.error().what()
                << std::endl;
      co_return;
    }

    auto texture = device.value()->createTexture({
        .type = kl::graphics::TextureType::e2D,
        .format = kl::graphics::Format::eR8G8B8A8Unorm,
        .extent = {512, 512, 1},
        .mipLevels = 4,
    });

    *texture;
  }

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
