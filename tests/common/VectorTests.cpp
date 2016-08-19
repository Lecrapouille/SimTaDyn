#include "VectorTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(VectorTests);

static int32_t coordinate[3] = { 4, 5, 6 };

static Vector3D v1;
static Vector3D v2(1.0f, 2.0f, 3.0f);
static Vector3D v3(1.0f, 2.0f);
static Vector3D v4(coordinate);
static Vector3D v5(1, 2, 3);

static inline void checkVector(const Vector3D& v, const float32_t x, const float32_t y, const float32_t z)
{
  CPPUNIT_ASSERT_EQUAL(v.x, x);
  CPPUNIT_ASSERT_EQUAL(v.y, y);
  CPPUNIT_ASSERT_EQUAL(v.z, z);
}

//--------------------------------------------------------------------------
void VectorTests::setUp()
{
}

//--------------------------------------------------------------------------
void VectorTests::tearDown()
{
}

//--------------------------------------------------------------------------
void VectorTests::testCreator()
{
  CPPUNIT_ASSERT_EQUAL(sizeof (Vector3D), 12U);
  checkVector(v2, 1.0f, 2.0f, 3.0f);
  checkVector(v3, 1.0f, 2.0f, 0.0f);
  checkVector(v4, 4.0f, 5.0f, 6.0f);
  checkVector(v5, 1.0f, 2.0f, 3.0f);

  std::cout << v2 << ", " << v3 << ", " << v4 << ", " << v5 << std::endl;
}

//--------------------------------------------------------------------------
void VectorTests::testSwap()
{
  v2.swap(v4);
  checkVector(v2, 4.0f, 5.0f, 6.0f);
  v2.swap(v4);
  checkVector(v2, 1.0f, 2.0f, 3.0f);
  v4.swap(v2);
  checkVector(v2, 4.0f, 5.0f, 6.0f);
  v4.swap(v2);
  checkVector(v2, 1.0f, 2.0f, 3.0f);
}

//--------------------------------------------------------------------------
void VectorTests::testEquality()
{
  CPPUNIT_ASSERT_EQUAL((v2 == v5), true);
  CPPUNIT_ASSERT_EQUAL((v3 == v2), false);
  CPPUNIT_ASSERT_EQUAL((v2 != v5), false);
  CPPUNIT_ASSERT_EQUAL((v3 != v2), true);
}

//--------------------------------------------------------------------------
void VectorTests::testArithmetic()
{
  const float32_t c_scalar = -2.0f;
  float32_t scalar = -2.0f;

  checkVector((v2 + v5 + v3), 3.0f, 6.0f, 6.0f);
  checkVector((v5 * 2), 2.0f, 4.0f, 6.0f);
  checkVector((v5 * -2), -2.0f, -4.0f, -6.0f);

  // FIXME
  //checkVector((-2.0f * v5), -2.0f, -4.0f, -6.0f);
  //checkVector((c_scalar * v5), -2.0f, -4.0f, -6.0f);
  checkVector((-v5 * 2), -2.0f, -4.0f, -6.0f);
  checkVector((-v5 * c_scalar), 2.0f, 4.0f, 6.0f);
  checkVector((-v5 * scalar), 2.0f, 4.0f, 6.0f);

  checkVector(Vector3D::ZERO + 4.0f, 4.0f, 4.0f, 4.0f);
  // div

  CPPUNIT_ASSERT_EQUAL(Vector3D::ZERO, Vector3D::UNIT_X - Vector3D::UNIT_X);
  checkVector(-Vector3D::UNIT_X, -1.0f, 0.0f, 0.0f);
  checkVector(+Vector3D::UNIT_X, +1.0f, 0.0f, 0.0f);
  +v1;
}

//--------------------------------------------------------------------------
void VectorTests::testCopy()
{
  v1 = v5;
  checkVector(v1, 1.0f, 2.0f, 3.0f);
  v1 = 42;
  checkVector(v1, 42.0f, 42.0f, 42.0f);
}

//--------------------------------------------------------------------------
void VectorTests::testOperations()
{
  checkVector(v2.min(Vector3D::UNIT_Z + 1.0f), 1.0f, 1.0f, 2.0f);
  checkVector(v2.max(Vector3D::UNIT_Y + 2.0f), 2.0f, 3.0f, 3.0f);
  checkVector(Vector3D::ZERO.midPoint(Vector3D::UNIT_Z), 0.0f, 0.0f, 0.5f);
  checkVector(Vector3D::ZERO.midPoint(Vector3D::UNIT_Y), 0.0f, 0.5f, 0.0f);
  checkVector(Vector3D::ZERO.midPoint(Vector3D::UNIT_X), 0.5f, 0.0f, 0.0f);
  checkVector(Vector3D::UNIT_X.midPoint(-Vector3D::UNIT_X), 0.0f, 0.0f, 0.0f);
  checkVector((-Vector3D::UNIT_X).midPoint(Vector3D::UNIT_X), 0.0f, 0.0f, 0.0f);
  checkVector(-Vector3D::UNIT_X.midPoint(Vector3D::UNIT_X), -1.0f, 0.0f, 0.0f);
  CPPUNIT_ASSERT_EQUAL(1.0f, Vector3D::UNIT_X.length());
  CPPUNIT_ASSERT_EQUAL(std::sqrt(3.0f), Vector3D::UNIT_SCALE.length());
  CPPUNIT_ASSERT_EQUAL(std::sqrt(3.0f), (-Vector3D::UNIT_SCALE).length());
  CPPUNIT_ASSERT_EQUAL(3.0f, Vector3D::UNIT_SCALE.squaredLength());
  CPPUNIT_ASSERT_EQUAL(0.0f, Vector3D::UNIT_X.dotProduct(Vector3D::UNIT_Y));
  CPPUNIT_ASSERT_EQUAL(3.0f, Vector3D::UNIT_SCALE.dotProduct(Vector3D::UNIT_SCALE));
  CPPUNIT_ASSERT_EQUAL(Vector3D::UNIT_Z, Vector3D::UNIT_X.crossProduct(Vector3D::UNIT_Y));
  CPPUNIT_ASSERT_EQUAL(-Vector3D::UNIT_Z, Vector3D::UNIT_Y.crossProduct(Vector3D::UNIT_X));
  v1 = Vector3D::UNIT_SCALE * 2.0f;
  CPPUNIT_ASSERT_EQUAL(true, v1.normalise());
  CPPUNIT_ASSERT_EQUAL(Vector3D::UNIT_SCALE * 1.0f/std::sqrt(3.0f), v1);

  CPPUNIT_ASSERT_EQUAL(1.0f, v1.distance(Vector3D::ZERO));
}
