#ifndef BASICARCTESTS_HPP_
#  define BASICARCTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "BasicGraph.hpp"
#undef protected
#undef private

class BasicArcTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(BasicArcTests);
  CPPUNIT_TEST(test);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void test();
};

#endif /* BASICARCTESTS_HPP_ */
