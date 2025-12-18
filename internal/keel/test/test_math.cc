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

TEST(MathTest, OverflowingAdd) {
  for (int16_t i = -128; i <= 127; ++i) {
    for (int16_t j = -128; j <= 127; ++j) {
      int16_t sum = i + j;
      int8_t expectedResult = static_cast<int8_t>(sum);
      bool expectedOverflow = (sum < -128 || sum > 127);
      auto [result, overflow] = Math::overflowingAdd<int8_t>(i, j);
      EXPECT_EQ(result, expectedResult);
      EXPECT_EQ(overflow, expectedOverflow);
    }
  }
}

TEST(MathTest, OverflowingAddUnsigned) {
  for (uint16_t i = 0; i <= 255; ++i) {
    for (uint16_t j = 0; j <= 255; ++j) {
      uint16_t sum = i + j;
      uint8_t expectedResult = static_cast<uint8_t>(sum);
      bool expectedOverflow = (sum > 255);
      auto [result, overflow] = Math::overflowingAdd<uint8_t>(i, j);
      EXPECT_EQ(result, expectedResult);
      EXPECT_EQ(overflow, expectedOverflow);
    }
  }
}

TEST(MathTest, CheckedAdd) {
  for (int16_t i = -128; i <= 127; ++i) {
    for (int16_t j = -128; j <= 127; ++j) {
      int16_t sum = i + j;
      bool overflow = (sum < -128 || sum > 127);
      auto result = Math::checkedAdd<int8_t>(i, j);
      if (overflow) {
        EXPECT_FALSE(result.has_value());
      } else {
        EXPECT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), static_cast<int8_t>(sum));
      }
    }
  }
}

TEST(MathTest, CheckedAddUnsigned) {
  for (uint16_t i = 0; i <= 255; ++i) {
    for (uint16_t j = 0; j <= 255; ++j) {
      uint16_t sum = i + j;
      bool overflow = (sum > 255);
      auto result = Math::checkedAdd<uint8_t>(i, j);
      if (overflow) {
        EXPECT_FALSE(result.has_value());
      } else {
        EXPECT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), static_cast<uint8_t>(sum));
      }
    }
  }
}

TEST(MathTest, WrappingAdd) {
  for (int16_t i = -128; i <= 127; ++i) {
    for (int16_t j = -128; j <= 127; ++j) {
      int8_t expected = static_cast<int8_t>(static_cast<uint8_t>(i) +
                                            static_cast<uint8_t>(j));
      EXPECT_EQ(Math::wrappingAdd<int8_t>(i, j), expected);
    }
  }
}

TEST(MathTest, WrappingAddUnsigned) {
  for (uint16_t i = 0; i <= 255; ++i) {
    for (uint16_t j = 0; j <= 255; ++j) {
      uint8_t expected = static_cast<uint8_t>(static_cast<uint16_t>(i) +
                                              static_cast<uint16_t>(j));
      EXPECT_EQ(Math::wrappingAdd<uint8_t>(i, j), expected);
    }
  }
}

TEST(MathTest, SaturatingAdd) {
  for (int16_t i = -128; i <= 127; ++i) {
    for (int16_t j = -128; j <= 127; ++j) {
      int16_t sum = i + j;
      int8_t expected =
          sum < -128 ? -128 : (sum > 127 ? 127 : static_cast<int8_t>(sum));
      EXPECT_EQ(Math::saturatingAdd<int8_t>(i, j), expected);
    }
  }
}

TEST(MathTest, SaturatingAddUnsigned) {
  for (uint16_t i = 0; i <= 255; ++i) {
    for (uint16_t j = 0; j <= 255; ++j) {
      uint16_t sum = i + j;
      uint8_t expected = sum > 255 ? 255 : static_cast<uint8_t>(sum);
      EXPECT_EQ(Math::saturatingAdd<uint8_t>(i, j), expected);
    }
  }
}