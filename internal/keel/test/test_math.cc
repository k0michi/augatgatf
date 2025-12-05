#include "kl/math/math.hh"
#include <gtest/gtest.h>
#include <numbers>

constexpr float kEps = 1e-4f;

using kl::math::Math;

TEST(MathTest, ToRadians) {
  EXPECT_NEAR(Math::toRadians(0.0f), 0.0f, kEps);
  EXPECT_NEAR(Math::toRadians(180.0f), static_cast<float>(std::numbers::pi),
              kEps);
  EXPECT_NEAR(Math::toRadians(90.0f),
              static_cast<float>(std::numbers::pi) / 2.0f, kEps);
}

TEST(MathTest, ToDegrees) {
  EXPECT_NEAR(Math::toDegrees(0.0f), 0.0f, kEps);
  EXPECT_NEAR(Math::toDegrees(static_cast<float>(std::numbers::pi)), 180.0f,
              kEps);
  EXPECT_NEAR(Math::toDegrees(static_cast<float>(std::numbers::pi) / 2.0f),
              90.0f, kEps);
}

TEST(MathTest, ClampIntegral) {
  EXPECT_EQ(Math::clamp(5, 1, 10), 5);
  EXPECT_EQ(Math::clamp(0, 1, 10), 1);
  EXPECT_EQ(Math::clamp(15, 1, 10), 10);
  EXPECT_EQ(Math::clamp(-5, -3, 3), -3);
  EXPECT_EQ(Math::clamp(5, -3, 3), 3);
}

TEST(MathTest, ClampIntegralInvalidArgs) {
  EXPECT_THROW(Math::clamp(5, 10, 1), std::invalid_argument);
  EXPECT_THROW(Math::clamp(0, 5, -5), std::invalid_argument);
}

TEST(MathTest, ClampFloatingPoint) {
  EXPECT_FLOAT_EQ(Math::clamp(5.0f, 1.0f, 10.0f), 5.0f);
  EXPECT_FLOAT_EQ(Math::clamp(0.0f, 1.0f, 10.0f), 1.0f);
  EXPECT_FLOAT_EQ(Math::clamp(15.0f, 1.0f, 10.0f), 10.0f);
  EXPECT_FLOAT_EQ(Math::clamp(-5.0f, -3.0f, 3.0f), -3.0f);
  EXPECT_FLOAT_EQ(Math::clamp(5.0f, -3.0f, 3.0f), 3.0f);
}

TEST(MathTest, ClampFloatingPointInvalidArgs) {
  EXPECT_THROW(Math::clamp(5.0f, 10.0f, 1.0f), std::invalid_argument);
  EXPECT_THROW(Math::clamp(0.0f, 5.0f, -5.0f), std::invalid_argument);
  EXPECT_THROW(Math::clamp(0.0f, std::nanf(""), 1.0f), std::invalid_argument);
  EXPECT_THROW(Math::clamp(0.0f, 1.0f, std::nanf("")), std::invalid_argument);
  EXPECT_THROW(Math::clamp(0.0f, 0.0f, -0.0f), std::invalid_argument);
}

TEST(MathTest, MixFloat) {
  EXPECT_FLOAT_EQ(Math::mix(0.0f, 1.0f, 0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Math::mix(0.0f, 1.0f, 1.0f), 1.0f);
  EXPECT_FLOAT_EQ(Math::mix(0.0f, 1.0f, 0.5f), 0.5f);
  EXPECT_FLOAT_EQ(Math::mix(-1.0f, 1.0f, 0.25f), -0.5f);
  EXPECT_FLOAT_EQ(Math::mix(0.0f, 1.0f, 2.0f), 2.0f);
}

TEST(MathTest, MixFloatBool) {
  EXPECT_FLOAT_EQ(Math::mix(1.0f, 2.0f, true), 2.0f);
  EXPECT_FLOAT_EQ(Math::mix(1.0f, 2.0f, false), 1.0f);
}

TEST(MathTest, MixIntBool) {
  EXPECT_EQ(Math::mix(10, 20, true), 20);
  EXPECT_EQ(Math::mix(10, 20, false), 10);
  EXPECT_EQ(Math::mix(-5, 5, true), 5);
  EXPECT_EQ(Math::mix(-5, 5, false), -5);
}

TEST(MathTest, MixBool) {
  EXPECT_EQ(Math::mix(false, true, true), true);
  EXPECT_EQ(Math::mix(false, true, false), false);
  EXPECT_EQ(Math::mix(true, false, true), false);
  EXPECT_EQ(Math::mix(true, false, false), true);
}

TEST(MathTest, Step) {
  EXPECT_FLOAT_EQ(Math::step(0.5f, 0.4f), 0.0f);
  EXPECT_FLOAT_EQ(Math::step(0.5f, 0.5f), 1.0f);
  EXPECT_FLOAT_EQ(Math::step(0.5f, 0.6f), 1.0f);
  EXPECT_FLOAT_EQ(Math::step(-1.0f, -2.0f), 0.0f);
  EXPECT_FLOAT_EQ(Math::step(-1.0f, 0.0f), 1.0f);
}

TEST(MathTest, Smoothstep) {
  EXPECT_FLOAT_EQ(Math::smoothstep(0.0f, 1.0f, -1.0f), 0.0f);
  EXPECT_NEAR(Math::smoothstep(0.0f, 1.0f, 0.0f), 0.0f, kEps);
  EXPECT_NEAR(Math::smoothstep(0.0f, 1.0f, 1.0f), 1.0f, kEps);
  EXPECT_FLOAT_EQ(Math::smoothstep(0.0f, 1.0f, 2.0f), 1.0f);
  EXPECT_NEAR(Math::smoothstep(0.0f, 1.0f, 0.5f), 0.5f, kEps);
  EXPECT_NEAR(Math::smoothstep(-1.0f, 1.0f, 0.0f), 0.5f, kEps);
}