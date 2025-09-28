#include <iostream>

#include "kl/concurrent/task.hh"
#include "kl/graphics/instance.hh"
#include "kl/math/matrix2x2.hh"
#include "kl/math/matrix3x3.hh"
#include "kl/math/matrix4x4.hh"
#include "kl/math/quaternion.hh"
#include "kl/math/vector2.hh"
#include "kl/math/vector3.hh"
#include "kl/math/vector4.hh"
#include "kl/platform/instance.hh"
#include "kl/platform/window.hh"

#include <algorithm>
#include <array>
#include <fstream>
#include <string>
#include <string_view>

#ifndef __EMSCRIPTEN__
#include <uv.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten/fetch.h>

struct FetchAwaiter {
  FetchAwaiter(std::string_view url) : mUrl(url) {}

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> handle) {
    mHandle = handle;
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = [](emscripten_fetch_t *fetch) {
      auto self = static_cast<FetchAwaiter *>(fetch->userData);
      self->mData.assign(reinterpret_cast<const std::byte *>(fetch->data),
                         reinterpret_cast<const std::byte *>(fetch->data) +
                             fetch->numBytes);
      self->mHandle.resume();
      emscripten_fetch_close(fetch);
    };
    attr.onerror = [](emscripten_fetch_t *fetch) {
      auto self = static_cast<FetchAwaiter *>(fetch->userData);
      self->mError = true;
      self->mHandle.resume();
      emscripten_fetch_close(fetch);
    };
    attr.userData = this;
    emscripten_fetch(&attr, mUrl.data());
  }

  std::vector<std::byte> await_resume() {
    if (mError) {
      throw std::runtime_error("Failed to fetch: " + mUrl);
    }
    return mData;
  }

private:
  std::string mUrl;
  std::vector<std::byte> mData;
  std::coroutine_handle<> mHandle;
  bool mError = false;
};
#endif

#ifdef __EMSCRIPTEN__
kl::concurrent::Task<std::vector<std::byte>> fetch(std::string_view url) {
  std::vector<std::byte> result = co_await FetchAwaiter(url);
  co_return result;
}
#endif

std::vector<std::byte> readFileBinary(std::string_view filename) {
  std::ifstream file(filename.data(), std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + std::string(filename));
  }
  std::vector<char> tmp((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
  std::vector<std::byte> buffer(tmp.size());
  std::transform(tmp.begin(), tmp.end(), buffer.begin(),
                 [](char c) { return static_cast<std::byte>(c); });
  return buffer;
}

kl::concurrent::Task<std::vector<std::byte>>
loadAsync(std::string_view filename) {
#ifdef __EMSCRIPTEN__
  auto data = co_await fetch(filename);
  co_return data;
#else
  co_return readFileBinary(filename);
#endif
}

struct Matrices {
  kl::math::Matrix4x4 projection;
  kl::math::Matrix4x4 view;
  kl::math::Matrix4x4 model;
};

std::vector<uint8_t> createDummyTexture(int size = 64, int blockSize = 8) {
  std::vector<uint8_t> data(size * size * 4);
  for (int y = 0; y < size; ++y) {
    for (int x = 0; x < size; ++x) {
      bool checker = ((x / blockSize) % 2) == ((y / blockSize) % 2);
      uint8_t color = checker ? 255 : 32;
      int idx = (y * size + x) * 4;
      data[idx + 0] = color; // R
      data[idx + 1] = color; // G
      data[idx + 2] = color; // B
      data[idx + 3] = 255;   // A
    }
  }
  return data;
}

kl::concurrent::Task<void> pseudoMain(int argc, char **argv) {
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

  auto vertShader = co_await loadAsync("shaders/texture.vert.spv");
  auto fragShader = co_await loadAsync("shaders/texture.frag.spv");
  auto program = device->createProgram({
      .shaders = {device
                      ->createShader({
                          .stage = kl::graphics::ShaderStage::eVertex,
                          .code = vertShader,
                          .entryPoint = "main",
                      })
                      .value(),
                  device
                      ->createShader({
                          .stage = kl::graphics::ShaderStage::eFragment,
                          .code = fragShader,
                          .entryPoint = "main",
                      })
                      .value()},
  });

  if (!program) {
    std::cerr << "Failed to create program: " << program.error().what()
              << std::endl;
    co_return;
  }

  auto texture = device
                     ->createTexture({
                         .type = kl::graphics::TextureType::e2D,
                         .format = kl::graphics::Format::eR8G8B8A8Unorm,
                         .extent = {512, 512, 1},
                         .mipLevels = 4,
                     })
                     .value();
  auto textureData = createDummyTexture(512, 32);
  context->writeTexture(texture, 0, {0, 0, 0}, {512, 512, 1},
                        std::as_bytes(std::span(textureData)),
                        /* srcRowLength */ 512,
                        /* srcImageHeight */ 512);
  context->generateMipmaps(texture);

  auto sampler = device->createSampler({}).value();

  auto buffer = device->createBuffer({
      .size = 1024,
  });

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

  auto uniformBuffer = device->createBuffer({
      .size = sizeof(Matrices),
      .usage = kl::graphics::BufferUsage::eUniformBuffer,
  });

  context->setRasterizationState(
      device
          ->createRasterizationState({
              .cullMode = kl::graphics::CullMode::eNone,
          })
          .value());

  while (!instance->shouldQuit()) {
    auto elapsed = co_await instance->waitFrame();
    context->setFramebuffer(swapchain->framebuffer());
    // context->setViewport(kl::graphics::Viewport{0, 0, 10, 10, 0, 1});
    // context->setScissorRect(kl::graphics::ScissorRect{
    //     .offset = {0, 0},
    //     .extent = {10, 10},
    // });
    context->clearColor({0.0f, 0.0f, 0.0f, 0.0f});
    context->clearDepthStencil(1.0f, 0);
    context->setProgram(program.value());
    context->setVertexBuffer(0, vertexBuffer.value(), 0);
    context->setVertexInputState(vertexInputState.value());
    context->setTexture(0, texture);
    context->setSampler(0, sampler);

    Matrices matrices{
        .projection = kl::math::Matrix4x4::perspective(
            45.0f * (3.14159265359f / 180.0f), 800.0f / 600.0f, 0.1f, 100.0f),
        .view = kl::math::Matrix4x4::lookAt(
            {0.0f, 0.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}),
        .model = kl::math::Matrix4x4::fromQuaternion(
            kl::math::Quaternion<float>::fromAxisAngle(
                {0.0f, .0f, 1.0f}, static_cast<float>(count) * 0.01f)),
    };

    context->writeBuffer(uniformBuffer.value(), 0,
                         std::as_bytes(std::span(&matrices, 1)));

    context->setUniformBuffer(0, uniformBuffer.value(), 0, sizeof(Matrices));
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

int main(int argc, char **argv) {
  pseudoMain(argc, argv);

#ifndef __EMSCRIPTEN__
  uv_loop_t *loop = uv_default_loop();
  uv_run(loop, UV_RUN_DEFAULT);
  uv_loop_close(loop);
#endif

  return 0;
}
