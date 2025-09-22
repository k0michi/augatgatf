#include <gtest/gtest.h>

#include "kl/math/matrix2x2.hh"

TEST(Matrix2x2, Determinant) {
  kl::math::Matrix2x2 mat{1.0f, 2.0f, 3.0f, 4.0f};
  EXPECT_FLOAT_EQ(mat.determinant(), -2.0f);
}

TEST(Matrix2x2, Inverse) {
  kl::math::Matrix2x2 mat{4.0f, 7.0f, 2.0f, 6.0f};
  auto invOpt = mat.inverse();
  ASSERT_TRUE(invOpt.has_value());
  kl::math::Matrix2x2 inv = invOpt.value();
  EXPECT_FLOAT_EQ(inv.m00, 0.6f);
  EXPECT_FLOAT_EQ(inv.m01, -0.7f);
  EXPECT_FLOAT_EQ(inv.m10, -0.2f);
  EXPECT_FLOAT_EQ(inv.m11, 0.4f);
}

TEST(Matrix2x2, InverseNonInvertible) {
  kl::math::Matrix2x2 mat{1.0f, 2.0f, 2.0f, 4.0f};
  auto invOpt = mat.inverse();
  EXPECT_FALSE(invOpt.has_value());
}