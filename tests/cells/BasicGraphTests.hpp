#ifndef BASICGRAPHTESTS_HPP_
#  define BASICGRAPHTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "BasicGraph.hpp"
#undef protected
#undef private

class BasicGraphTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(BasicGraphTests);
  CPPUNIT_TEST(test);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void test();
};

#endif /* BASICGRAPHTESTS_HPP_ */
