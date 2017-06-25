#include "MatrixTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(MatrixTests);

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
static Matrix33f one(1);
static Matrix33f two(2.0f);
static Matrix33f four(4);
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
static Vector3f v(5.0f, 4.0f, 3.0f);
static Vector3f A_times_v(22, 58, 94);
static Vector3f v_times_A(42, 54, 66);

//--------------------------------------------------------------------------
template <typename T, uint32_t r, uint32_t c>
static inline void compareMatrix_(Matrix<T,r,c> const& a, Matrix<T,r,c> const& b, const bool expected)
{
  std::cout << "Comparing: " << std::endl << a
            << "With: " << std::endl << b
            << std::endl;

  Matrix<bool,r,c> res = (a == b);
  for (uint32_t i = 0; i < r * c; ++i)
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
template <typename T, uint32_t r, uint32_t c>
static inline void isTrueMatrix(Matrix<T,r,c> const& a, Matrix<T,r,c> const& b)
{
  compareMatrix_(a, b, true);
}

//--------------------------------------------------------------------------
template <typename T, uint32_t r, uint32_t c>
static inline void isFalseMatrix(Matrix<T,r,c> const& a, Matrix<T,r,c> const& b)
{
  compareMatrix_(a, b, false);
}

//--------------------------------------------------------------------------
static inline void checkVector3f(Vector3f const& v, const float x, const float y, const float z)
{
  CPPUNIT_ASSERT_EQUAL(x, v.x);
  CPPUNIT_ASSERT_EQUAL(y, v.y);
  CPPUNIT_ASSERT_EQUAL(z, v.z);
}

//--------------------------------------------------------------------------
void MatrixTests::setUp()
{
}

//--------------------------------------------------------------------------
void MatrixTests::tearDown()
{
}

//--------------------------------------------------------------------------
void MatrixTests::testCreator()
{
  uint32_t rows = 0;
  uint32_t cols = 0;

  B4.size(rows, cols);
  CPPUNIT_ASSERT_EQUAL(4U, rows);
  CPPUNIT_ASSERT_EQUAL(4U, cols);

  B.size(rows, cols);
  CPPUNIT_ASSERT_EQUAL(3U, rows);
  CPPUNIT_ASSERT_EQUAL(3U, cols);

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

  one += 1.0f;
  isTrueMatrix(two, one);
  one -= 1.0f;
  isTrueMatrix(one, one);
  one *= 4.0f;
  isTrueMatrix(four, one);
  one /= 4.0f;
  isTrueMatrix(one, one);

  isTrueMatrix(minusA, A * -1.0f);
  isTrueMatrix(minusA, -1.0f * A);
  isTrueMatrix(minusA, -A);
  isTrueMatrix(four, 4.0f * one);
  isTrueMatrix(four, one * 4.0f);
  isTrueMatrix(one, 4.0f / four);
  isTrueMatrix(one, four / 4.0f);

  checkVector3f(A * v, A_times_v.x, A_times_v.y, A_times_v.z);
  checkVector3f(v * A, v_times_A.x, v_times_A.y, v_times_A.z);

  Vector3f v1(v);
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
}
