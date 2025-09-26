#include <gtest/gtest.h>

#include "kl/math/matrix4x4.hh"

TEST(Matrix4x4, Constructor) {
  kl::math::Matrix<float, 4, 4> mat{1.0f,  2.0f,  3.0f,  4.0f,  5.0f,  6.0f,
                                    7.0f,  8.0f,  9.0f,  10.0f, 11.0f, 12.0f,
                                    13.0f, 14.0f, 15.0f, 16.0f};
  EXPECT_FLOAT_EQ(mat.m00, 1.0f);
  EXPECT_FLOAT_EQ(mat.m01, 2.0f);
  EXPECT_FLOAT_EQ(mat.m02, 3.0f);
  EXPECT_FLOAT_EQ(mat.m03, 4.0f);
  EXPECT_FLOAT_EQ(mat.m10, 5.0f);
  EXPECT_FLOAT_EQ(mat.m11, 6.0f);
  EXPECT_FLOAT_EQ(mat.m12, 7.0f);
  EXPECT_FLOAT_EQ(mat.m13, 8.0f);
  EXPECT_FLOAT_EQ(mat.m20, 9.0f);
  EXPECT_FLOAT_EQ(mat.m21, 10.0f);
  EXPECT_FLOAT_EQ(mat.m22, 11.0f);
  EXPECT_FLOAT_EQ(mat.m23, 12.0f);
  EXPECT_FLOAT_EQ(mat.m30, 13.0f);
  EXPECT_FLOAT_EQ(mat.m31, 14.0f);
  EXPECT_FLOAT_EQ(mat.m32, 15.0f);
  EXPECT_FLOAT_EQ(mat.m33, 16.0f);
}