#ifndef KL_ASSETS_PNGREADER_HH
#define KL_ASSETS_PNGREADER_HH

#include <expected>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "kl/assets/image.hh"
#include "kl/io/async_stream.hh"
#include "kl/io/stream_util.hh"

namespace kl::assets {
template <kl::io::AsyncReadable T>
kl::concurrent::Task<std::expected<Image, std::runtime_error>>
readPNG(std::shared_ptr<kl::io::Loop> loop, T &stream) {
  auto result = co_await kl::io::readAll(loop, stream);

  if (!result) {
    co_return std::unexpected(std::runtime_error("Failed to read PNG data"));
  }

  int width, height, channels;

  auto is16Bit = stbi_is_16_bit_from_memory(
      reinterpret_cast<const stbi_uc *>(result->data()),
      static_cast<int>(result->size()));

  void *imageData = nullptr;

  if (is16Bit) {
    imageData = stbi_load_16_from_memory(
        reinterpret_cast<const stbi_uc *>(result->data()),
        static_cast<int>(result->size()), &width, &height, &channels, 0);
  } else {
    imageData = stbi_load_from_memory(
        reinterpret_cast<const stbi_uc *>(result->data()),
        static_cast<int>(result->size()), &width, &height, &channels, 0);
  }

  if (!imageData) {
    co_return std::unexpected(std::runtime_error(
        "Failed to decode PNG image: " + std::string(stbi_failure_reason())));
  }

  std::size_t dataSize = static_cast<std::size_t>(width) *
                         static_cast<std::size_t>(height) *
                         static_cast<std::size_t>(channels) * (is16Bit ? 2 : 1);

  std::vector<std::byte> data(dataSize);
  std::memcpy(data.data(), imageData, dataSize);

  stbi_image_free(imageData);

  auto imageExpected = Image::create(
      kl::common::Extent2<std::uint32_t>{static_cast<std::uint32_t>(width),
                                         static_cast<std::uint32_t>(height)},
      (channels == 1
           ? (is16Bit ? ColorFormat::eR16Unorm : ColorFormat::eR8Unorm)
           : (channels == 2
                  ? (is16Bit ? ColorFormat::eR16G16Unorm
                             : ColorFormat::eR8G8Unorm)
                  : (channels == 3 ? (is16Bit ? ColorFormat::eR16G16B16Unorm
                                              : ColorFormat::eR8G8B8Unorm)
                                   : (is16Bit ? ColorFormat::eR16G16B16A16Unorm
                                              : ColorFormat::eR8G8B8A8Unorm)))),
      std::span<const std::byte>(data));

  if (!imageExpected.has_value()) {
    co_return std::unexpected(imageExpected.error());
  }

  co_return *imageExpected;
}
} // namespace kl::assets
#endif