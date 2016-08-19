#ifndef VECTORTESTS_HPP_
#  define VECTORTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Vector3D.hpp"

class VectorTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(VectorTests);
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

#endif /* VECTORTESTS_HPP_ */
