#include "Matrix.h"

#include <gtest/gtest.h>

TEST(Any_Dim_Matrix, Constructor) {
  // Matrix<int, 0> invalid;
  Matrix<int, 1> vec;
  Matrix<int, 2> mat;
}
