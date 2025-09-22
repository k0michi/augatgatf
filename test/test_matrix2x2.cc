#include <gtest/gtest.h>

#include "kl/math/matrix2x2.hh"

TEST(Matrix2x2, Constructor) {
  kl::math::Matrix2x2 mat{1.0f, 2.0f, 3.0f, 4.0f};
  EXPECT_FLOAT_EQ(mat.m00, 1.0f);
  EXPECT_FLOAT_EQ(mat.m01, 2.0f);
  EXPECT_FLOAT_EQ(mat.m10, 3.0f);
  EXPECT_FLOAT_EQ(mat.m11, 4.0f);
}

TEST(Matrix2x2, AccessColumns) {
  kl::math::Vector2 col0{1.0f, 3.0f};
  kl::math::Vector2 col1{2.0f, 4.0f};
  kl::math::Matrix2x2 mat{col0, col1};
  EXPECT_FLOAT_EQ(mat.col0.x, 1.0f);
  EXPECT_FLOAT_EQ(mat.col0.y, 3.0f);
  EXPECT_FLOAT_EQ(mat.col1.x, 2.0f);
  EXPECT_FLOAT_EQ(mat.col1.y, 4.0f);
}

TEST(Matrix2x2, IndexOperator) {
  kl::math::Matrix2x2 mat{1.0f, 2.0f, 3.0f, 4.0f};
  EXPECT_FLOAT_EQ(mat[0][0], 1.0f);
  EXPECT_FLOAT_EQ(mat[0][1], 3.0f);
  EXPECT_FLOAT_EQ(mat[1][0], 2.0f);
  EXPECT_FLOAT_EQ(mat[1][1], 4.0f);
}

TEST(Matrix2x2, IndexOperatorConst) {
  const kl::math::Matrix2x2 mat{1.0f, 2.0f, 3.0f, 4.0f};
  EXPECT_FLOAT_EQ(mat[0][0], 1.0f);
  EXPECT_FLOAT_EQ(mat[0][1], 3.0f);
  EXPECT_FLOAT_EQ(mat[1][0], 2.0f);
  EXPECT_FLOAT_EQ(mat[1][1], 4.0f);
}

TEST(Matrix2x2, Transpose) {
  kl::math::Matrix2x2 mat{1.0f, 2.0f, 3.0f, 4.0f};
  kl::math::Matrix2x2 transposed = mat.transpose();
  EXPECT_FLOAT_EQ(transposed.m00, 1.0f);
  EXPECT_FLOAT_EQ(transposed.m01, 3.0f);
  EXPECT_FLOAT_EQ(transposed.m10, 2.0f);
  EXPECT_FLOAT_EQ(transposed.m11, 4.0f);
}

TEST(Matrix2x2, Addition) {
  kl::math::Matrix2x2 mat1{1.0f, 2.0f, 3.0f, 4.0f};
  kl::math::Matrix2x2 mat2{5.0f, 6.0f, 7.0f, 8.0f};
  kl::math::Matrix2x2 result = mat1 + mat2;
  EXPECT_FLOAT_EQ(result.m00, 6.0f);
  EXPECT_FLOAT_EQ(result.m01, 8.0f);
  EXPECT_FLOAT_EQ(result.m10, 10.0f);
  EXPECT_FLOAT_EQ(result.m11, 12.0f);
}

TEST(Matrix2x2, AdditionAssignment) {
  kl::math::Matrix2x2 mat1{1.0f, 2.0f, 3.0f, 4.0f};
  kl::math::Matrix2x2 mat2{5.0f, 6.0f, 7.0f, 8.0f};
  mat1 += mat2;
  EXPECT_FLOAT_EQ(mat1.m00, 6.0f);
  EXPECT_FLOAT_EQ(mat1.m01, 8.0f);
  EXPECT_FLOAT_EQ(mat1.m10, 10.0f);
  EXPECT_FLOAT_EQ(mat1.m11, 12.0f);
}

TEST(Matrix2x2, Subtraction) {
  kl::math::Matrix2x2 mat1{5.0f, 6.0f, 7.0f, 8.0f};
  kl::math::Matrix2x2 mat2{1.0f, 2.0f, 3.0f, 4.0f};
  kl::math::Matrix2x2 result = mat1 - mat2;
  EXPECT_FLOAT_EQ(result.m00, 4.0f);
  EXPECT_FLOAT_EQ(result.m01, 4.0f);
  EXPECT_FLOAT_EQ(result.m10, 4.0f);
  EXPECT_FLOAT_EQ(result.m11, 4.0f);
}

TEST(Matrix2x2, SubtractionAssignment) {
  kl::math::Matrix2x2 mat1{5.0f, 6.0f, 7.0f, 8.0f};
  kl::math::Matrix2x2 mat2{1.0f, 2.0f, 3.0f, 4.0f};
  mat1 -= mat2;
  EXPECT_FLOAT_EQ(mat1.m00, 4.0f);
  EXPECT_FLOAT_EQ(mat1.m01, 4.0f);
  EXPECT_FLOAT_EQ(mat1.m10, 4.0f);
  EXPECT_FLOAT_EQ(mat1.m11, 4.0f);
}

TEST(Matrix2x2, ScalarMultiplication) {
  kl::math::Matrix2x2 mat{1.0f, 2.0f, 3.0f, 4.0f};
  kl::math::Matrix2x2 result = mat * 2.0f;
  EXPECT_FLOAT_EQ(result.m00, 2.0f);
  EXPECT_FLOAT_EQ(result.m01, 4.0f);
  EXPECT_FLOAT_EQ(result.m10, 6.0f);
  EXPECT_FLOAT_EQ(result.m11, 8.0f);
}

TEST(Matrix2x2, ScalarMultiplicationAssignment) {
  kl::math::Matrix2x2 mat{1.0f, 2.0f, 3.0f, 4.0f};
  mat *= 2.0f;
  EXPECT_FLOAT_EQ(mat.m00, 2.0f);
  EXPECT_FLOAT_EQ(mat.m01, 4.0f);
  EXPECT_FLOAT_EQ(mat.m10, 6.0f);
  EXPECT_FLOAT_EQ(mat.m11, 8.0f);
}

TEST(Matrix2x2, VectorMultiplication) {
  kl::math::Matrix2x2 mat{1.0f, 2.0f, 3.0f, 4.0f};
  kl::math::Vector2 vec{1.0f, 1.0f};
  kl::math::Vector2 result = mat * vec;
  EXPECT_FLOAT_EQ(result.x, 3.0f);
  EXPECT_FLOAT_EQ(result.y, 7.0f);
}

TEST(Matrix2x2, MatrixMultiplication) {
  kl::math::Matrix2x2 mat1{1.0f, 2.0f, 3.0f, 4.0f};
  kl::math::Matrix2x2 mat2{5.0f, 6.0f, 7.0f, 8.0f};
  kl::math::Matrix2x2 result = mat1 * mat2;
  EXPECT_FLOAT_EQ(result.m00, 19.0f);
  EXPECT_FLOAT_EQ(result.m01, 22.0f);
  EXPECT_FLOAT_EQ(result.m10, 43.0f);
  EXPECT_FLOAT_EQ(result.m11, 50.0f);
}

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