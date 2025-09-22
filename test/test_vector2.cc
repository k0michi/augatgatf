#include <gtest/gtest.h>

#include <kl/math/vector2.hh>

TEST(Vector2, DefaultConstructor) {
  kl::math::Vector2 v;
  EXPECT_FLOAT_EQ(v.x, 0.0f);
  EXPECT_FLOAT_EQ(v.y, 0.0f);
}

TEST(Vector2, ParameterizedConstructor) {
  kl::math::Vector2 v{3.0f, 4.0f};
  EXPECT_FLOAT_EQ(v.x, 3.0f);
  EXPECT_FLOAT_EQ(v.y, 4.0f);
}