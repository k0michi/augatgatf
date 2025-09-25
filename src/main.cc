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

#include <array>
#include <fstream>
#include <spirv_glsl.hpp>

std::vector<std::byte> readFileBinary(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  std::vector<char> tmp((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
  std::vector<std::byte> buffer(tmp.size());
  std::transform(tmp.begin(), tmp.end(), buffer.begin(),
                 [](char c) { return static_cast<std::byte>(c); });
  return buffer;
}

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
  auto window =
      instance
          ->createWindow(kl::platform::WindowDescriptor{"My Window", 800, 600})
          .value();

  auto graphicsInstance =
      kl::graphics::Instance::create(kl::graphics::InstanceDescriptor{})
          .value();
  auto device =
      graphicsInstance->createDevice(kl::graphics::DeviceDescriptor{}).value();
  auto swapchain =
      device
          ->createSwapchain({
              .window = window,
              .colorFormat = kl::graphics::Format::eR8G8B8A8Unorm,
              .depthStencilFormat = kl::graphics::Format::eD24UnormS8Uint,
              .doubleBuffered = true,
          })
          .value();
  auto context = device->createContext({}).value();

  auto rasterizationState = device->createRasterizationState({});

  auto program = device->createProgram({
      .shaders = {device
                      ->createShader({
                          .stage = kl::graphics::ShaderStage::eVertex,
                          .code = readFileBinary("shaders/hello.vert.spv"),
                          .entryPoint = "main",
                      })
                      .value(),
                  device
                      ->createShader({
                          .stage = kl::graphics::ShaderStage::eFragment,
                          .code = readFileBinary("shaders/hello.frag.spv"),
                          .entryPoint = "main",
                      })
                      .value()},
  });

  if (!program) {
    std::cerr << "Failed to create program: " << program.error().what()
              << std::endl;
    co_return;
  }

  auto texture = device->createTexture({
      .type = kl::graphics::TextureType::e2D,
      .format = kl::graphics::Format::eR8G8B8A8Unorm,
      .extent = {512, 512, 1},
      .mipLevels = 4,
  });

  auto buffer = device->createBuffer({
      .size = 1024,
  });

  auto sampler = device->createSampler({});

  auto vertexInputState = device->createVertexInputState({
      .bindings = {{
          .binding = 0,
          .stride = 12,
          .inputRate = kl::graphics::VertexInputRate::eVertex,
      }},
      .attributes = {{
          .location = 0,
          .binding = 0,
          .format = kl::graphics::Format::eR32G32B32Sfloat,
          .offset = 0,
      }},
  });

  std::array<float, 9> vertexData = {
      0.0f,  0.5f,
      0.0f, // Vertex 1: x, y, z
      -0.5f, -0.5f,
      0.0f, // Vertex 3: x, y, z
      0.5f,  -0.5f,
      0.0f, // Vertex 2: x, y, z
  };

  auto vertexBuffer = device->createBuffer({
      .size = sizeof(vertexData),
      .usage = kl::graphics::BufferUsage::eVertexBuffer,
  });

  context->writeBuffer(vertexBuffer.value(), 0,
                       std::as_bytes(std::span(vertexData)));

  int32_t count = 0;

  while (!instance->shouldQuit()) {
    auto elapsed = co_await instance->waitFrame();
    context->setFramebuffer(swapchain->framebuffer());
    // context->setViewport(kl::graphics::Viewport{0, 0, 10, 10, 0, 1});
    // context->setScissorRect(kl::graphics::ScissorRect{
    //     .offset = {0, 0},
    //     .extent = {10, 10},
    // });
    // context->clearColor({1.0f, 1.0f, 0.0f, 1.0f});
    // context->clearDepthStencil(1.0f, 0);
    context->setProgram(program.value());
    context->setVertexBuffer(0, vertexBuffer.value(), 0);
    context->setVertexInputState(vertexInputState.value());
    context->draw(kl::graphics::PrimitiveTopology::eTriangleList, 3, 1, 0, 0);
    swapchain->present(1);
    instance->pollEvents();

    auto error = SDL_GetError();
    if (error[0] != '\0') {
      std::cerr << "SDL Error: " << error << std::endl;
      SDL_ClearError();
    }

    // std::cout << elapsed << std::endl;
    count++;
  }
}

int main() {
  main_async();
  return 0;
}
