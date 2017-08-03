#ifndef GRAPHALGOTESTS_HPP_
#  define GRAPHALGOTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "GraphAlgorithm.hpp"
#undef protected
#undef private

class GraphAlgoTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(GraphAlgoTests);
  CPPUNIT_TEST(test);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void test();
};

#endif /* GRAPHALGOTESTS_HPP_ */
