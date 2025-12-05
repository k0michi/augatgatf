#include "kl/common/colors.hh"
#include <cmath>
#include <gtest/gtest.h>

using kl::common::rgbToHSL;
using kl::math::Vector4;

bool Vector4Near(const Vector4 &a, const Vector4 &b, float eps = 1e-4f) {
  return std::fabs(a.x - b.x) < eps && std::fabs(a.y - b.y) < eps &&
         std::fabs(a.z - b.z) < eps && std::fabs(a.w - b.w) < eps;
}

#define EXPECT_VECTOR4_NEAR(a, b, eps) EXPECT_TRUE(Vector4Near((a), (b), (eps)))

constexpr float kEps = 1e-4f;

TEST(RGBToHSLTest, Red) {
  Vector4 rgb{1.0f, 0.0f, 0.0f, 1.0f};
  Vector4 expected{0.0f, 1.0f, 0.5f, 1.0f};
  auto hsl = rgbToHSL(rgb);
  EXPECT_VECTOR4_NEAR(hsl, expected, kEps);
}

TEST(RGBToHSLTest, Green) {
  Vector4 rgb{0.0f, 1.0f, 0.0f, 1.0f};
  Vector4 expected{120.0f, 1.0f, 0.5f, 1.0f};
  auto hsl = rgbToHSL(rgb);
  EXPECT_VECTOR4_NEAR(hsl, expected, kEps);
}

TEST(RGBToHSLTest, Blue) {
  Vector4 rgb{0.0f, 0.0f, 1.0f, 1.0f};
  Vector4 expected{240.0f, 1.0f, 0.5f, 1.0f};
  auto hsl = rgbToHSL(rgb);
  EXPECT_VECTOR4_NEAR(hsl, expected, kEps);
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
  Vector4 expected{1.0f, 0.0f, 0.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_VECTOR4_NEAR(rgb, expected, kEps);
}

TEST(HSLToRGBTest, Green) {
  Vector4 hsl{120.0f, 100.0f, 50.0f, 1.0f};
  Vector4 expected{0.0f, 1.0f, 0.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_VECTOR4_NEAR(rgb, expected, kEps);
}

TEST(HSLToRGBTest, Blue) {
  Vector4 hsl{240.0f, 100.0f, 50.0f, 1.0f};
  Vector4 expected{0.0f, 0.0f, 1.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_VECTOR4_NEAR(rgb, expected, kEps);
}

TEST(HSLToRGBTest, White) {
  Vector4 hsl{0.0f, 0.0f, 100.0f, 1.0f};
  Vector4 expected{1.0f, 1.0f, 1.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_VECTOR4_NEAR(rgb, expected, kEps);
}

TEST(HSLToRGBTest, Black) {
  Vector4 hsl{0.0f, 0.0f, 0.0f, 1.0f};
  Vector4 expected{0.0f, 0.0f, 0.0f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_VECTOR4_NEAR(rgb, expected, kEps);
}

TEST(HSLToRGBTest, Gray) {
  Vector4 hsl{0.0f, 0.0f, 50.0f, 1.0f};
  Vector4 expected{0.5f, 0.5f, 0.5f, 1.0f};
  auto rgb = hslToRGB(hsl);
  EXPECT_VECTOR4_NEAR(rgb, expected, kEps);
}