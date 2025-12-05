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