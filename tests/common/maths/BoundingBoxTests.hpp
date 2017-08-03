#ifndef BOUNDINGBOXTESTS_HPP_
#  define BOUNDINGBOXTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "BoundingBox.tpp"
#undef protected
#undef private

class BoundingBoxTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(BoundingBoxTests);
  CPPUNIT_TEST(testCreator);
  CPPUNIT_TEST(testSwap);
  CPPUNIT_TEST(testEquality);
  CPPUNIT_TEST(testArithmetic);
  CPPUNIT_TEST(testCopy);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testCreator();
  void testSwap();
  void testEquality();
  void testArithmetic();
  void testCopy();
  void testOperations();
};

#endif /* BOUNDINGBOXTESTS_HPP_ */
