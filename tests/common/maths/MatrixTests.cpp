//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "MatrixTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(MatrixTests);

// Expected results are computed by the ScicosLab tool
static Matrix33f I3 =
  {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };
static Matrix33f ID3(matrix::Identity);
static Matrix33f A3 =
  {
    1.0f, 2.0f, 3.0f,
    4.0f, 5.0f, 6.0f,
    7.0f, 8.0f, 9.0f
  };
static Matrix33f HA3 =
  {
    1.0f, 4.0f, 9.0f,
    16.0f, 25.0f, 36.0f,
    49.0f, 64.0f, 81.0f
  };
static Matrix44f A4 =
  {
    1.0f,   2.0f,  3.0f,  4.0f,
    5.0f,   6.0f,  7.0f,  8.0f,
    9.0f,  10.0f, 11.0f, 12.0f,
    13.0f, 14.0f, 15.0f, 16.0f
  };
static Matrix33f AA(A4);
static Matrix33f A(A3);
static Matrix33f Atrunc =
  {
    1.0f,   2.0f,  3.0f,
    5.0f,   6.0f,  7.0f,
    9.0f,  10.0f, 11.0f
  };
static Matrix33f minusA =
  {
    -1.0f, -2.0f, -3.0f,
    -4.0f, -5.0f, -6.0f,
    -7.0f, -8.0f, -9.0f
  };
static Matrix33f B3 =
  {
    1.0f, 4.0f, 7.0f,
    2.0f, 5.0f, 8.0f,
    3.0f, 6.0f, 9.0f
  };
static Matrix44f B4 =
  {
    1.0f, 5.0f, 9.0f,  13.0f,
    2.0f, 6.0f, 10.0f, 14.0f,
    3.0f, 7.0f, 11.0f, 15.0f,
    4.0f, 8.0f, 12.0f, 16.0f
  };
static Matrix33f BB(B4);
static Matrix33f B(B3);
static Matrix33f Btrunc =
  {
    1.0f, 5.0f, 9.0f,
    2.0f, 6.0f, 10.0f,
    3.0f, 7.0f, 11.0f,
    4.0f, 8.0f, 12.0f
  };
static Matrix33f O = { 0.0f };
static Matrix33i one(1);
static Matrix33i two(2);
static Matrix33i four(4);
static Matrix33f A_times_B =
  {
    14,    32,     50,
    32,    77,     122,
    50,    122,    194
  };
static Matrix33f B_times_A =
  {
    66,    78,     90,
    78,    93,     108,
    90,    108,    126
  };
static Matrix33f A_plus_B =
  {
    2,     6,     10,
    6,     10,    14,
    10,    14,    18
  };
static Matrix33f A_minus_B =
  {
    0,  - 2,  - 4,
    2,    0,  - 2,
    4,    2,    0
  };
static Matrix33f B_minus_A =
  {
      0,    2,    4,
    - 2,    0,    2,
    - 4,  - 2,    0
  };
static Matrix44f M4(one);
static Matrix44f M =
  {
    1, 1, 1, 0,
    1, 1, 1, 0,
    1, 1, 1, 0,
    0, 0, 0, 0
  };
static Matrix33b Mfalse(false);
static Matrix33b Mtrue(true);
static Vector3f V(5.0f, 4.0f, 3.0f);
static Vector3f A_times_v(22, 58, 94);
static Vector3f v_times_A(42, 54, 66);

//--------------------------------------------------------------------------
template <typename T, size_t r, size_t c>
static void compareMatricesEps(Matrix<T,r,c> const &a, Matrix<T,r,c> const &b)
{
  for (size_t i = 0_z; i < r * c; ++i)
    {
      CPPUNIT_ASSERT_EQUAL(true, maths::abs(a.m_data[i] - b.m_data[i]) < T(0.000001));
    }
}

//--------------------------------------------------------------------------
template <typename T, size_t r, size_t c>
static void compareMatricesUlps(Matrix<T,r,c> const &a, Matrix<T,r,c> const &b)
{
  for (size_t i = 0_z; i < r * c; ++i)
    {
      CPPUNIT_ASSERT_EQUAL(true, maths::almostEqual(a.m_data[i], b.m_data[i]));
    }
}

//--------------------------------------------------------------------------
template <typename T, size_t r, size_t c>
static void compareMatrix_(Matrix<T,r,c> const& a, Matrix<T,r,c> const& b, const bool expected)
{
  std::cout << "Comparing: " << std::endl << a
            << "With: " << std::endl << b
            << std::endl;

  Matrix<bool,r,c> res = (a == b);
  for (size_t i = 0_z; i < r * c; ++i)
    {
      //CPPUNIT_ASSERT_EQUAL(true, res.m_data[i]);
      if (expected != res.m_data[i])
        {
          std::cout << "KOOOOOOO" << std::endl;
          exit(1);
        }
    }
}

//--------------------------------------------------------------------------
template <typename T, size_t r, size_t c>
static void isTrueMatrix(Matrix<T,r,c> const& a, Matrix<T,r,c> const& b)
{
  compareMatrix_(a, b, true);
}

//--------------------------------------------------------------------------
template <typename T, size_t r, size_t c>
static void isFalseMatrix(Matrix<T,r,c> const& a, Matrix<T,r,c> const& b)
{
  compareMatrix_(a, b, false);
}

//--------------------------------------------------------------------------
static void checkVector3f(Vector3f const& v, const float x, const float y, const float z)
{
  CPPUNIT_ASSERT_EQUAL(x, v.x);
  CPPUNIT_ASSERT_EQUAL(y, v.y);
  CPPUNIT_ASSERT_EQUAL(z, v.z);
}

//--------------------------------------------------------------------------
static void checkAlmostVectorEps(Vector3g const& v, const double x, const double y, const double z)
{
  CPPUNIT_ASSERT_EQUAL(true, maths::abs(x - v.x) < 0.000001);
  CPPUNIT_ASSERT_EQUAL(true, maths::abs(y - v.y) < 0.000001);
  CPPUNIT_ASSERT_EQUAL(true, maths::abs(z - v.z) < 0.000001);
}

//--------------------------------------------------------------------------
void MatrixTests::setUp()
{
  std::cout << "*** MatrixTests ***************************************" << std::endl;
}

//--------------------------------------------------------------------------
void MatrixTests::tearDown()
{
}

//--------------------------------------------------------------------------
void MatrixTests::testCreator()
{
  size_t rows = 0_z;
  size_t cols = 0_z;

  B4.size(rows, cols);
  CPPUNIT_ASSERT_EQUAL(4_z, rows);
  CPPUNIT_ASSERT_EQUAL(4_z, cols);

  B.size(rows, cols);
  CPPUNIT_ASSERT_EQUAL(3_z, rows);
  CPPUNIT_ASSERT_EQUAL(3_z, cols);

  // Check accessor
  checkVector3f(I3[0], 1.0f, 0.0f, 0.0f);
  checkVector3f(I3[1], 0.0f, 1.0f, 0.0f);
  checkVector3f(I3[2], 0.0f, 0.0f, 1.0f);

  // Check accessor
  checkVector3f(A3[0], 1.0f, 2.0f, 3.0f);
  checkVector3f(A3[1], 4.0f, 5.0f, 6.0f);
  checkVector3f(A3[2], 7.0f, 8.0f, 9.0f);
}

//--------------------------------------------------------------------------
void MatrixTests::testSwap()
{
}

//--------------------------------------------------------------------------
void MatrixTests::testEquality()
{
  isFalseMatrix(one, two);
  isTrueMatrix(Mtrue, one < four);
  isTrueMatrix(Mtrue, one <= four);
  isTrueMatrix(Mtrue, one <= one);
  isTrueMatrix(Mfalse, one < one);
  isTrueMatrix(Mfalse, one > one);
  isTrueMatrix(Mtrue, one >= one);
  isTrueMatrix(Mfalse, one > four);
  isTrueMatrix(Mfalse, one >= four);
  isTrueMatrix(Mfalse, one == four);
  isTrueMatrix(Mtrue, one != four);
  isTrueMatrix(Mtrue, !Mfalse);
  isTrueMatrix(Mfalse, !Mtrue);

  isTrueMatrix(Mfalse, Mtrue & Mfalse);
  isTrueMatrix(Mtrue, Mtrue | Mfalse);
  isTrueMatrix(Mfalse, Mtrue ^ Mtrue);

  isTrueMatrix(I3, ID3);
  isTrueMatrix(A, A3);
  isTrueMatrix(Atrunc, AA);
  isTrueMatrix(B, B3);
  isTrueMatrix(Btrunc, BB);
  isTrueMatrix(M, M4);
  isTrueMatrix(matrix::transpose(A), B);
  isTrueMatrix(matrix::transpose(B), A);

  CPPUNIT_ASSERT_EQUAL(true, matrix::allTrue(Matrix33b(true)));
  CPPUNIT_ASSERT_EQUAL(false, matrix::allFalse(Matrix33b(true)));
  CPPUNIT_ASSERT_EQUAL(true, matrix::allFalse(Matrix33b(false)));
  CPPUNIT_ASSERT_EQUAL(false, matrix::allTrue(Matrix33b(false)));
  CPPUNIT_ASSERT_EQUAL(true, matrix::allTrue(matrix::transpose(A) == B));
  CPPUNIT_ASSERT_EQUAL(false, matrix::allTrue(A == B));
  CPPUNIT_ASSERT_EQUAL(false, matrix::allFalse(matrix::transpose(A) == B));
  CPPUNIT_ASSERT_EQUAL(false, matrix::allFalse(A == B));
}

//--------------------------------------------------------------------------
void MatrixTests::testArithmetic()
{
  isTrueMatrix(O, O * I3);
  isTrueMatrix(O, I3 * O);
  isTrueMatrix(A, A * I3);
  isTrueMatrix(B, I3 * B);
  isTrueMatrix(A_times_B, A * B);
  isTrueMatrix(B_times_A, B * A);

  isTrueMatrix(I3, O + I3);
  isTrueMatrix(I3, I3 + O);
  isTrueMatrix(I3 + A, A + I3);
  isTrueMatrix(A_plus_B, A + B);
  isTrueMatrix(A_plus_B, B + A);
  isTrueMatrix(A_minus_B, A - B);
  isTrueMatrix(B_minus_A, B - A);

    one += 1;
  isTrueMatrix(two, one);
    one -= 1;
    one *= 4;
  isTrueMatrix(four, one);
    one /= 4;
    one += 1;
    isTrueMatrix(two, one);
    one -= 1;

  isTrueMatrix(minusA, A * -1.0f);
  isTrueMatrix(minusA, -1.0f * A);
  isTrueMatrix(minusA, -A);
    isTrueMatrix(four, 4 * one);
    isTrueMatrix(four, one * 4);
    isTrueMatrix(one, 4 / four);
    isTrueMatrix(one, four / 4);

    checkVector3f(A * V, A_times_v.x, A_times_v.y, A_times_v.z);
    checkVector3f(V * A, v_times_A.x, v_times_A.y, v_times_A.z);

    Vector3f v1(V);
  v1 *= A;
  checkVector3f(v1, v_times_A.x, v_times_A.y, v_times_A.z);
  Matrix33f C(A);
  C *= B;
  isTrueMatrix(C, A_times_B);

  isTrueMatrix(HA3, matrix::Hprod(A3, A3));

  CPPUNIT_ASSERT_EQUAL(true, matrix::allTrue(matrix::compare(I3, I3)));
  //CPPUNIT_ASSERT_EQUAL(true, matrix::allTrue(matrix::compare(I3, I3 * 0.00001f)));

  CPPUNIT_ASSERT_EQUAL(107.0f, matrix::trace(HA3));
  CPPUNIT_ASSERT_EQUAL(true, matrix::isDiagonal(I3));
  //CPPUNIT_ASSERT_EQUAL(true, matrix::isDiagonal(I3 * 0.0001f));
  CPPUNIT_ASSERT_EQUAL(false, matrix::isDiagonal(HA3));
  CPPUNIT_ASSERT_EQUAL(true, matrix::isSymmetric(I3));
  //CPPUNIT_ASSERT_EQUAL(true, matrix::isSymetric(I3 * 0.0001f));
  CPPUNIT_ASSERT_EQUAL(false, matrix::isSymmetric(HA3));
}

//--------------------------------------------------------------------------
void MatrixTests::testCopy()
{
  Matrix33f O3 = O;

  O = I3;
  isTrueMatrix(I3, (O * O));
  O = O3;
  isTrueMatrix(O, (O * I3));
  isTrueMatrix(I3, (I3 + O));

  // Swap rows
  Matrix33f tmp = A3;
  checkVector3f(tmp[0], 1.0f, 2.0f, 3.0f);
  checkVector3f(tmp[1], 4.0f, 5.0f, 6.0f);
  checkVector3f(tmp[2], 7.0f, 8.0f, 9.0f);

  CPPUNIT_ASSERT_EQUAL(true, matrix::swapRows(tmp, 0U, 2U));
  checkVector3f(tmp[0], 7.0f, 8.0f, 9.0f);
  checkVector3f(tmp[1], 4.0f, 5.0f, 6.0f);
  checkVector3f(tmp[2], 1.0f, 2.0f, 3.0f);

  CPPUNIT_ASSERT_EQUAL(true, matrix::swapRows(tmp, 0U, 0U));
  checkVector3f(tmp[0], 7.0f, 8.0f, 9.0f);
  checkVector3f(tmp[1], 4.0f, 5.0f, 6.0f);
  checkVector3f(tmp[2], 1.0f, 2.0f, 3.0f);

  CPPUNIT_ASSERT_EQUAL(true, matrix::swapRows(tmp, 10U, 10U));
  checkVector3f(tmp[0], 7.0f, 8.0f, 9.0f);
  checkVector3f(tmp[1], 4.0f, 5.0f, 6.0f);
  checkVector3f(tmp[2], 1.0f, 2.0f, 3.0f);

  CPPUNIT_ASSERT_EQUAL(false, matrix::swapRows(tmp, 0U, 10U));
  checkVector3f(tmp[0], 7.0f, 8.0f, 9.0f);
  checkVector3f(tmp[1], 4.0f, 5.0f, 6.0f);
  checkVector3f(tmp[2], 1.0f, 2.0f, 3.0f);
}

//--------------------------------------------------------------------------
void MatrixTests::testOperations()
{
  // Random matrix
  Matrix44g Ra =
    {
      -0.5003796,   0.1910551,  -0.1043591,  -0.3966362,
       1.1937458,  -1.3189198,   0.2973099,   0.5163254,
      -1.5206395,   0.9307226,   0.5308515,   0.0075659,
      1.8655072,  -0.8575199,  -1.5404673,   1.0422456,
    };
  // LU decomposition of Ra. Expected result.
  Matrix44g U =
    {
      1.8655072,  -0.8575199,  -1.5404673,   1.0422456,
      0.0,        -0.7701892,   1.2830613,  -0.1506119,
      0.0,         0.0,        -0.5824496,  -0.1094599,
      0.0,         0.0,         0.0,         0.8754921
    };
  // LU decomposition of Ra. Expected result.
  Matrix44g L =
    {
       1.0,         0.0,          0.0,          0.0,
       0.6399042,   1.0,          0.0,          0.0,
      -0.2682271,   0.0505785,    1.0,          0.0,
      -0.8151346,  -0.3008722,    0.5816799,    1.0,
    };

  Matrix44g LL(11111.0), UU(22222.0), P(333.3); // Init with random values
  matrix::LUdecomposition(Ra, LL, UU, P);
  maths::maxUlps = 1U;
  compareMatricesEps(LL, L);
  compareMatricesEps(UU, U);

  Vector3g a(3.0f, -24.0f, 30.0f);
  Matrix33g B = {
      9.0f,   -36.0f,    30.0f,
    -36.0f,   192.0f,  -180.0f,
     30.0f,  -180.0f,   180.0f
  };
  // ScicosLab: x = a / B
  Vector3g x(matrix::LUsolve(B, a));
  Vector3g Z(x * B - a);

  checkAlmostVectorEps(x, 1.0, 1.0, 1.0); // Close to 1
  checkAlmostVectorEps(Z, 0.0, 0.0, 0.0); // Close to 0
}
