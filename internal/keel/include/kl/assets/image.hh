#ifndef KL_ASSETS_IMAGE_HH
#define KL_ASSETS_IMAGE_HH

#include <cstdint>
#include <expected>
#include <span>
#include <stdexcept>
#include <vector>

#include "kl/common/extent2.hh"
#include "kl/math/vector4.hh"

namespace kl::assets {
// TODO: Integrate with kl::graphics::Format
enum class ColorFormat {
  eR8Unorm,
  eR8G8Unorm,
  eR8G8B8Unorm,
  eR8G8B8A8Unorm,

  eR8Srbg,
  eR8G8Srgb,
  eR8G8B8Srgb,
  eR8G8B8A8Srgb,

  eR16Unorm,
  eR16G16Unorm,
  eR16G16B16Unorm,
  eR16G16B16A16Unorm,

  eR16Srbg,
  eR16G16Srbg,
  eR16G16B16Srbg,
  eR16G16B16A16Srbg,

  eR32Float,
  eR32G32Float,
  eR32G32B32Float,
  eR32G32B32A32Float,
};

struct Image {
private:
  static std::size_t getBytesPerPixel(ColorFormat format) {
    switch (format) {
    case ColorFormat::eR8Unorm:
    case ColorFormat::eR8Srbg:
      return 1;
    case ColorFormat::eR8G8Unorm:
    case ColorFormat::eR8G8Srgb:
      return 2;
    case ColorFormat::eR8G8B8Unorm:
    case ColorFormat::eR8G8B8Srgb:
      return 3;
    case ColorFormat::eR8G8B8A8Unorm:
    case ColorFormat::eR8G8B8A8Srgb:
      return 4;
    case ColorFormat::eR16Unorm:
    case ColorFormat::eR16Srbg:
      return 2;
    case ColorFormat::eR16G16Unorm:
    case ColorFormat::eR16G16Srbg:
      return 4;
    case ColorFormat::eR16G16B16Unorm:
    case ColorFormat::eR16G16B16Srbg:
      return 6;
    case ColorFormat::eR16G16B16A16Unorm:
    case ColorFormat::eR16G16B16A16Srbg:
      return 8;
    case ColorFormat::eR32Float:
      return 4;
    case ColorFormat::eR32G32Float:
      return 8;
    case ColorFormat::eR32G32B32Float:
      return 12;
    case ColorFormat::eR32G32B32A32Float:
      return 16;
    default:
      return 0;
    }
  }

  static std::size_t getRowPitch(kl::common::Extent2<std::uint32_t> extent,
                                 ColorFormat format) {
    return extent.width * getBytesPerPixel(format);
  }

  static std::size_t getImageSize(kl::common::Extent2<std::uint32_t> extent,
                                  ColorFormat format) {
    return getRowPitch(extent, format) * extent.height;
  }

public:
  Image() = default;
  ~Image() = default;
  Image(const Image &) = default;
  Image &operator=(const Image &) = default;
  Image(Image &&) noexcept = default;
  Image &operator=(Image &&) noexcept = default;

  static std::expected<Image, std::runtime_error>
  create(kl::common::Extent2<std::uint32_t> extent, ColorFormat format,
         std::span<const std::byte> data) {
    Image img;
    img.extent_ = extent;
    img.format_ = format;

    if (data.size() != getImageSize(extent, format)) {
      return std::unexpected(std::runtime_error(
          "Data size does not match the expected image size"));
    }

    img.data_.assign(data.begin(), data.end());
    return img;
  }

  const kl::common::Extent2<std::uint32_t> &extent() const { return extent_; }

  ColorFormat format() const { return format_; }

  std::span<const std::byte> asBytes() const { return data_; }

private:
  kl::common::Extent2<std::uint32_t> extent_{0, 0};
  ColorFormat format_{ColorFormat::eR8G8B8A8Unorm};
  std::vector<std::byte> data_{};
};
} // namespace kl::assets
#endif