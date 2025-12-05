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
