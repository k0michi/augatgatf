#include "kl/common/colors.hh"
#include <cmath>
#include <gtest/gtest.h>

using kl::common::rgbToHSL;
using kl::math::Vector4;

constexpr float kEps = 1e-4f;

TEST(RGBToHSLTest, Red) {
  Vector4 rgb{1.0f, 0.0f, 0.0f, 1.0f};
  auto hsl = rgbToHSL(rgb);
  EXPECT_NEAR(hsl.x, 0.0f, kEps);
  EXPECT_NEAR(hsl.y, 1.0f, kEps);
  EXPECT_NEAR(hsl.z, 0.5f, kEps);
  EXPECT_NEAR(hsl.w, 1.0f, kEps);
}

TEST(RGBToHSLTest, Green) {
  Vector4 rgb{0.0f, 1.0f, 0.0f, 1.0f};
  auto hsl = rgbToHSL(rgb);
  EXPECT_NEAR(hsl.x, 120.0f, kEps);
  EXPECT_NEAR(hsl.y, 1.0f, kEps);
  EXPECT_NEAR(hsl.z, 0.5f, kEps);
}

TEST(RGBToHSLTest, Blue) {
  Vector4 rgb{0.0f, 0.0f, 1.0f, 1.0f};
  auto hsl = rgbToHSL(rgb);
  EXPECT_NEAR(hsl.x, 240.0f, kEps);
  EXPECT_NEAR(hsl.y, 1.0f, kEps);
  EXPECT_NEAR(hsl.z, 0.5f, kEps);
}

TEST(RGBToHSLTest, White) {
  Vector4 rgb{1.0f, 1.0f, 1.0f, 1.0f};
  auto hsl = rgbToHSL(rgb);
  EXPECT_TRUE(std::isnan(hsl.x));
  EXPECT_TRUE(std::isnan(hsl.y));
  EXPECT_NEAR(hsl.z, 1.0f, kEps);
}

TEST(RGBToHSLTest, Black) {
  Vector4 rgb{0.0f, 0.0f, 0.0f, 1.0f};
  auto hsl = rgbToHSL(rgb);
  EXPECT_TRUE(std::isnan(hsl.x));
  EXPECT_TRUE(std::isnan(hsl.y));
  EXPECT_NEAR(hsl.z, 0.0f, kEps);
}

TEST(RGBToHSLTest, Gray) {
  Vector4 rgb{0.5f, 0.5f, 0.5f, 1.0f};
  auto hsl = rgbToHSL(rgb);
  EXPECT_TRUE(std::isnan(hsl.x));
  EXPECT_TRUE(std::isnan(hsl.y));
  EXPECT_NEAR(hsl.z, 0.5f, kEps);
}

using kl::common::hslToRGB;

TEST(HSLToRGBTest, Red) {
  Vector4 hsl{0.0f, 100.0f, 50.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_NEAR(rgb.x, 1.0f, kEps);
  EXPECT_NEAR(rgb.y, 0.0f, kEps);
  EXPECT_NEAR(rgb.z, 0.0f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

TEST(HSLToRGBTest, Green) {
  Vector4 hsl{120.0f, 100.0f, 50.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_NEAR(rgb.x, 0.0f, kEps);
  EXPECT_NEAR(rgb.y, 1.0f, kEps);
  EXPECT_NEAR(rgb.z, 0.0f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

TEST(HSLToRGBTest, Blue) {
  Vector4 hsl{240.0f, 100.0f, 50.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_NEAR(rgb.x, 0.0f, kEps);
  EXPECT_NEAR(rgb.y, 0.0f, kEps);
  EXPECT_NEAR(rgb.z, 1.0f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

TEST(HSLToRGBTest, White) {
  Vector4 hsl{0.0f, 0.0f, 100.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_NEAR(rgb.x, 1.0f, kEps);
  EXPECT_NEAR(rgb.y, 1.0f, kEps);
  EXPECT_NEAR(rgb.z, 1.0f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

TEST(HSLToRGBTest, Black) {
  Vector4 hsl{0.0f, 0.0f, 0.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_NEAR(rgb.x, 0.0f, kEps);
  EXPECT_NEAR(rgb.y, 0.0f, kEps);
  EXPECT_NEAR(rgb.z, 0.0f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

TEST(HSLToRGBTest, Gray) {
  Vector4 hsl{0.0f, 0.0f, 50.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_NEAR(rgb.x, 0.5f, kEps);
  EXPECT_NEAR(rgb.y, 0.5f, kEps);
  EXPECT_NEAR(rgb.z, 0.5f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

using kl::common::hwbToRGB;
using kl::common::rgbToHWB;

TEST(RGBToHWBTest, Red) {
  Vector4 rgb{1.0f, 0.0f, 0.0f, 1.0f};
  auto hwb = rgbToHWB(rgb);
  EXPECT_NEAR(hwb.x, 0.0f, kEps);
  EXPECT_NEAR(hwb.y, 0.0f, kEps);
  EXPECT_NEAR(hwb.z, 0.0f, kEps);
  EXPECT_NEAR(hwb.w, 1.0f, kEps);
}

TEST(RGBToHWBTest, Green) {
  Vector4 rgb{0.0f, 1.0f, 0.0f, 1.0f};
  auto hwb = rgbToHWB(rgb);
  EXPECT_NEAR(hwb.x, 120.0f, kEps);
  EXPECT_NEAR(hwb.y, 0.0f, kEps);
  EXPECT_NEAR(hwb.z, 0.0f, kEps);
}

TEST(RGBToHWBTest, Blue) {
  Vector4 rgb{0.0f, 0.0f, 1.0f, 1.0f};
  auto hwb = rgbToHWB(rgb);
  EXPECT_NEAR(hwb.x, 240.0f, kEps);
  EXPECT_NEAR(hwb.y, 0.0f, kEps);
  EXPECT_NEAR(hwb.z, 0.0f, kEps);
}

TEST(RGBToHWBTest, White) {
  Vector4 rgb{1.0f, 1.0f, 1.0f, 1.0f};
  auto hwb = rgbToHWB(rgb);
  EXPECT_TRUE(std::isnan(hwb.x));
  EXPECT_NEAR(hwb.y, 100.0f, kEps);
  EXPECT_NEAR(hwb.z, 0.0f, kEps);
}

TEST(RGBToHWBTest, Black) {
  Vector4 rgb{0.0f, 0.0f, 0.0f, 1.0f};
  auto hwb = rgbToHWB(rgb);
  EXPECT_TRUE(std::isnan(hwb.x));
  EXPECT_NEAR(hwb.y, 0.0f, kEps);
  EXPECT_NEAR(hwb.z, 100.0f, kEps);
}

TEST(RGBToHWBTest, Gray) {
  Vector4 rgb{0.5f, 0.5f, 0.5f, 1.0f};
  auto hwb = rgbToHWB(rgb);
  EXPECT_TRUE(std::isnan(hwb.x));
  EXPECT_NEAR(hwb.y, 50.0f, kEps);
  EXPECT_NEAR(hwb.z, 50.0f, kEps);
}

TEST(HWBToRGBTest, Red) {
  Vector4 hwb{0.0f, 0.0f, 0.0f, 1.0f};
  auto rgb = hwbToRGB(hwb);
  EXPECT_NEAR(rgb.x, 1.0f, kEps);
  EXPECT_NEAR(rgb.y, 0.0f, kEps);
  EXPECT_NEAR(rgb.z, 0.0f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

TEST(HWBToRGBTest, Green) {
  Vector4 hwb{120.0f, 0.0f, 0.0f, 1.0f};
  auto rgb = hwbToRGB(hwb);
  EXPECT_NEAR(rgb.x, 0.0f, kEps);
  EXPECT_NEAR(rgb.y, 1.0f, kEps);
  EXPECT_NEAR(rgb.z, 0.0f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

TEST(HWBToRGBTest, Blue) {
  Vector4 hwb{240.0f, 0.0f, 0.0f, 1.0f};
  auto rgb = hwbToRGB(hwb);
  EXPECT_NEAR(rgb.x, 0.0f, kEps);
  EXPECT_NEAR(rgb.y, 0.0f, kEps);
  EXPECT_NEAR(rgb.z, 1.0f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

TEST(HWBToRGBTest, White) {
  Vector4 hwb{0.0f, 100.0f, 0.0f, 1.0f};
  auto rgb = hwbToRGB(hwb);
  EXPECT_NEAR(rgb.x, 1.0f, kEps);
  EXPECT_NEAR(rgb.y, 1.0f, kEps);
  EXPECT_NEAR(rgb.z, 1.0f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

TEST(HWBToRGBTest, Black) {
  Vector4 hwb{0.0f, 0.0f, 100.0f, 1.0f};
  auto rgb = hwbToRGB(hwb);
  EXPECT_NEAR(rgb.x, 0.0f, kEps);
  EXPECT_NEAR(rgb.y, 0.0f, kEps);
  EXPECT_NEAR(rgb.z, 0.0f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}

TEST(HWBToRGBTest, Gray) {
  Vector4 hwb{0.0f, 50.0f, 50.0f, 1.0f};
  auto rgb = hwbToRGB(hwb);
  EXPECT_NEAR(rgb.x, 0.5f, kEps);
  EXPECT_NEAR(rgb.y, 0.5f, kEps);
  EXPECT_NEAR(rgb.z, 0.5f, kEps);
  EXPECT_NEAR(rgb.w, 1.0f, kEps);
}