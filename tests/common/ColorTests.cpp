#include "ColorTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(ColorTests);

static Color c1;
static Color c2(0.5f, 0.5f, 0.5f, 1.0f);
static Color c3(255U, 255U, 255U, 255U);
static Color c4(1.1f, 1.1f, 1.1f, 1.1f);
static Color c5 = Color::Black;
static Color c6 = Color::Red;

static inline void checkColor(const Color& c, const float32_t r, const float32_t g, const float32_t b, const float32_t a)
{
  CPPUNIT_ASSERT_EQUAL(r, c.r);
  CPPUNIT_ASSERT_EQUAL(g, c.g);
  CPPUNIT_ASSERT_EQUAL(b, c.b);
  CPPUNIT_ASSERT_EQUAL(a, c.a);
}

//--------------------------------------------------------------------------
void ColorTests::setUp()
{
}

//--------------------------------------------------------------------------
void ColorTests::tearDown()
{
}

//--------------------------------------------------------------------------
void ColorTests::testCreator()
{
  checkColor(c1, 0.0f, 0.0f, 0.0f, 1.0f);
  checkColor(c2, 0.5f, 0.5f, 0.5f, 1.0f);
  checkColor(c3, 1.0f, 1.0f, 1.0f, 1.0f);
  checkColor(c4, 1.0f, 1.0f, 1.0f, 1.0f);
  checkColor(c5, 0.0f, 0.0f, 0.0f, 1.0f);
  checkColor(c6, 1.0f, 0.0f, 0.0f, 1.0f);
}

//--------------------------------------------------------------------------
void ColorTests::testSwap()
{
  c5.swap(c6);
  CPPUNIT_ASSERT_EQUAL(Color::Red, c5);
  CPPUNIT_ASSERT_EQUAL(Color::Black, c6);

  c5.swap(c6);
  CPPUNIT_ASSERT_EQUAL(Color::Red, c6);
  CPPUNIT_ASSERT_EQUAL(Color::Black, c5);

  c5.swap(c6);
  CPPUNIT_ASSERT_EQUAL(Color::Red, c5);
  CPPUNIT_ASSERT_EQUAL(Color::Black, c6);

  c6.swap(c5);
  CPPUNIT_ASSERT_EQUAL(Color::Red, c6);
  CPPUNIT_ASSERT_EQUAL(Color::Black, c5);
}

//--------------------------------------------------------------------------
void ColorTests::testEquality()
{
  CPPUNIT_ASSERT_EQUAL(true, (c5 == Color::Black));
  CPPUNIT_ASSERT_EQUAL(true, (c5 == c1));
  CPPUNIT_ASSERT_EQUAL(true, (c5 != Color::Red));
}

//--------------------------------------------------------------------------
void ColorTests::testArithmetic()
{
  CPPUNIT_ASSERT_EQUAL(Color(0.5f, 0.5f, 0.5f, 1.0f), c1 + 0.5f);
  CPPUNIT_ASSERT_EQUAL(Color(0.5f * 0.5f, 0.5f * 0.5f, 0.5f * 0.5f, 1.0f * 0.5f), c2 * 0.5f);
}

//--------------------------------------------------------------------------
void ColorTests::testCopy()
{
  c1 = c5;
  CPPUNIT_ASSERT_EQUAL(true, (c1 == Color::Black));

  CPPUNIT_ASSERT_EQUAL(true, (c1 != c2));

  c1 = 0.5f;
  checkColor(c1, 0.5f, 0.5f, 0.5f, 1.0f);
  CPPUNIT_ASSERT_EQUAL(true, (c1 == c2));

  c1 = 0.25f;
  checkColor(c1, 0.25f, 0.25f, 0.25f, 1.0f);
  CPPUNIT_ASSERT_EQUAL(true, (c1 != c2));

  c1 = c2;
  CPPUNIT_ASSERT_EQUAL(true, (c1 == c2));
}

//--------------------------------------------------------------------------
void ColorTests::testOperations()
{
  c1.r = -1.0f;
  c1.constrain();
  CPPUNIT_ASSERT_EQUAL(0.0f, c1.r);

  c1.r = -1.0f;
  c1.g = 2.0f;
  c1.b = 2.0f;
  c1.a = -1.0f;
  c2 = Color::constrain(c1);
  std::cerr << c2 << std::endl;
  checkColor(c2, 0.0f, 1.0f, 1.0f, 0.0f);
}
