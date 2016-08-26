#include "BoundingBoxTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(BoundingBoxTests);

static AABB a1, a5;
static AABB a2(Vector3D(1,1,1), Vector3D(2,2,2));
static AABB a3 = AABB::AABB_UNIT_SCALE;
static AABB a4 = AABB::AABB_ZERO;
static AABB a6 = AABB::AABB_INFINITE;

//--------------------------------------------------------------------------
static inline void checkBox(const AABB& aabb, const Vector3D& v1, const Vector3D& v2)
{
  CPPUNIT_ASSERT_EQUAL(v1, aabb.bbmin);
  CPPUNIT_ASSERT_EQUAL(v2, aabb.bbmax);
}

//--------------------------------------------------------------------------
void BoundingBoxTests::setUp()
{
}

//--------------------------------------------------------------------------
void BoundingBoxTests::tearDown()
{
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testCreator()
{
  std::string msg = "";

  //std::cout << "AABB size " << sizeof (AABB) << std::endl;
  CPPUNIT_ASSERT_EQUAL(6U * sizeof (float32_t), sizeof (AABB));

  //std::cout << a1 << a2 << a3 << a4 << std::endl;

  msg = "";
  CPPUNIT_ASSERT_EQUAL(true, a1.isWellFormed(msg));
  CPPUNIT_ASSERT_EQUAL(std::string(""), msg);

  CPPUNIT_ASSERT_EQUAL(true, a2.isWellFormed(msg));
  CPPUNIT_ASSERT_EQUAL(std::string(""), msg);

  CPPUNIT_ASSERT_EQUAL(true, a3.isWellFormed(msg));
  CPPUNIT_ASSERT_EQUAL(std::string(""), msg);

  CPPUNIT_ASSERT_EQUAL(true, a4.isWellFormed(msg));
  CPPUNIT_ASSERT_EQUAL(std::string(""), msg);

  CPPUNIT_ASSERT_EQUAL(true, a6.isWellFormed(msg));
  CPPUNIT_ASSERT_EQUAL(std::string(""), msg);

  CPPUNIT_ASSERT_THROW(a5.setBox(Vector3D(2,2,2), Vector3D(1,1,1)), std::out_of_range);
  a5.setBox(Vector3D(1,1,1), Vector3D(2,2,2));
  CPPUNIT_ASSERT_EQUAL(a5, a2);
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testSwap()
{
  a1.swap(a2);
  checkBox(a1, Vector3D(1,1,1), Vector3D(2,2,2));
  checkBox(a2, Vector3D::ZERO, Vector3D::UNIT_SCALE);

  a1.swap(a2);
  checkBox(a2, Vector3D(1,1,1), Vector3D(2,2,2));
  checkBox(a1, Vector3D::ZERO, Vector3D::UNIT_SCALE);

  a2.swap(a1);
  checkBox(a1, Vector3D(1,1,1), Vector3D(2,2,2));
  checkBox(a2, Vector3D::ZERO, Vector3D::UNIT_SCALE);

  a2.swap(a1);
  checkBox(a2, Vector3D(1,1,1), Vector3D(2,2,2));
  checkBox(a1, Vector3D::ZERO, Vector3D::UNIT_SCALE);
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testEquality()
{
  CPPUNIT_ASSERT_EQUAL(false, (a1 == a2));
  CPPUNIT_ASSERT_EQUAL(true, (a1 != a2));
  CPPUNIT_ASSERT_EQUAL(true, (a1 != a6));
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testArithmetic()
{
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testCopy()
{
  /* AABB old;

  old = a1;

  a1 = a2;
  CPPUNIT_ASSERT_EQUAL((a1 == a2), true);

  a1 = AABB::AABB_UNIT_SCALE;
  CPPUNIT_ASSERT_EQUAL((a1 == a3), true);

  a1 = old;
  checkBox(a1, Vector3D::ZERO, Vector3D::UNIT_SCALE);*/
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testOperations()
{
  CPPUNIT_ASSERT_EQUAL(Vector3D(1.5f, 1.5f, 1.5f), a2.centerPoint());
  CPPUNIT_ASSERT_EQUAL(1.0f, a2.volume());
  CPPUNIT_ASSERT_EQUAL(4.0f * 4.0f * 4.0f, (a2 * 4.0f).volume());

  a5 =  a2 * 4.0f;
  CPPUNIT_ASSERT_EQUAL(true, (a5 > a2));
  CPPUNIT_ASSERT_EQUAL(false, (a5 < a2));
  CPPUNIT_ASSERT_EQUAL(true, (a2 >= a3));
  CPPUNIT_ASSERT_EQUAL(true, (a2 <= a3));
  CPPUNIT_ASSERT_EQUAL(false, (a2 == a3));
  CPPUNIT_ASSERT_EQUAL(true, (a6 > a3));

  CPPUNIT_ASSERT_EQUAL(true, a2.contains(a2.centerPoint()));
  CPPUNIT_ASSERT_EQUAL(false, a2.contains(a2.centerPoint() * 3));
  CPPUNIT_ASSERT_EQUAL(true, a6.contains(a2.centerPoint() * 3));

  CPPUNIT_ASSERT_EQUAL(false, a2.collides(a4));
  CPPUNIT_ASSERT_EQUAL(true, a2.collides(AABB(Vector3D(2,2,2), Vector3D(3,3,3))));
  CPPUNIT_ASSERT_EQUAL(true, a6.collides(a4));

  CPPUNIT_ASSERT_EQUAL(a3, a3.intersection(a3));
  CPPUNIT_ASSERT_EQUAL(a3, a3.merge(a3));
  CPPUNIT_ASSERT_EQUAL(a6, a6.intersection(a6));
  CPPUNIT_ASSERT_EQUAL(a6, a6.merge(a6));
  CPPUNIT_ASSERT_EQUAL(a6, a6.merge(a3));
  CPPUNIT_ASSERT_EQUAL(a6, a3.merge(a6));
  CPPUNIT_ASSERT_EQUAL(a3, a6.intersection(a3));
  CPPUNIT_ASSERT_EQUAL(a3, a3.intersection(a6));
  checkBox(a2.merge(a3), Vector3D::ZERO, Vector3D(2,2,2));
  checkBox(a2.intersection(a3), Vector3D(1,1,1), Vector3D(1,1,1));
  checkBox(a5.intersection(a3), Vector3D::ZERO, Vector3D::ZERO);

  CPPUNIT_ASSERT_EQUAL(true, a4.collides(a4));
  CPPUNIT_ASSERT_EQUAL(false, a4.collides(a2));
  CPPUNIT_ASSERT_EQUAL(false, a2.collides(a4));
  CPPUNIT_ASSERT_EQUAL(true, a2.collides(AABB(Vector3D(2,2,2), Vector3D(3,3,3))));
  CPPUNIT_ASSERT_EQUAL(true, a2.collides(Vector3D(1.5f, 1.5f, 1.5f)));

  CPPUNIT_ASSERT_EQUAL(true, a6.collides(a4));
  CPPUNIT_ASSERT_EQUAL(true, a4.collides(a6));

  CPPUNIT_ASSERT_EQUAL(std::numeric_limits<float32_t>::infinity(), a6.volume());
  CPPUNIT_ASSERT_EQUAL(Vector3D(std::numeric_limits<float32_t>::infinity(),
                                std::numeric_limits<float32_t>::infinity(),
                                std::numeric_limits<float32_t>::infinity()),
                       a6.dimension());
}
