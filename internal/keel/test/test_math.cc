#include "kl/math/math.hh"
#include <gtest/gtest.h>
#include <numbers>

constexpr float kEps = 1e-4f;

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