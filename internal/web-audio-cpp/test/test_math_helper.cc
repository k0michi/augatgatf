#include <gtest/gtest.h>

#include <web_audio.hh>

TEST(TestMathHelper, NextPowerOfTwo) {
  EXPECT_EQ(web_audio::detail::MathHelper::nextPowerOfTwo(0), 1);
  EXPECT_EQ(web_audio::detail::MathHelper::nextPowerOfTwo(1), 1);
  EXPECT_EQ(web_audio::detail::MathHelper::nextPowerOfTwo(2), 2);
  EXPECT_EQ(web_audio::detail::MathHelper::nextPowerOfTwo(3), 4);
  EXPECT_EQ(web_audio::detail::MathHelper::nextPowerOfTwo(4), 4);
  EXPECT_EQ(web_audio::detail::MathHelper::nextPowerOfTwo(5), 8);
  EXPECT_EQ(web_audio::detail::MathHelper::nextPowerOfTwo(15), 16);
  EXPECT_EQ(web_audio::detail::MathHelper::nextPowerOfTwo(16), 16);
  EXPECT_EQ(web_audio::detail::MathHelper::nextPowerOfTwo(17), 32);
}

TEST(TestMathHelper, NormalizeDegrees) {
  EXPECT_NEAR(web_audio::detail::MathHelper::normalizeDegrees(0.0), 0.0, 1e-10);
  EXPECT_NEAR(web_audio::detail::MathHelper::normalizeDegrees(360.0), 0.0,
              1e-10);
  EXPECT_NEAR(web_audio::detail::MathHelper::normalizeDegrees(720.0), 0.0,
              1e-10);
  EXPECT_NEAR(web_audio::detail::MathHelper::normalizeDegrees(450.0), 90.0,
              1e-10);
  EXPECT_NEAR(web_audio::detail::MathHelper::normalizeDegrees(-90.0), 270.0,
              1e-10);
  EXPECT_NEAR(web_audio::detail::MathHelper::normalizeDegrees(-450.0), 270.0,
              1e-10);
}