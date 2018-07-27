#ifndef TRANSFORMATIONTESTS_HPP_
#  define TRANSFORMATIONTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "Transformation.tpp"
#include "Movable.tpp"
#undef protected
#undef private

class TransformationTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(TransformationTests);
  CPPUNIT_TEST(tests);
  CPPUNIT_TEST(movable);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void tests();
  void movable();
};

#endif
