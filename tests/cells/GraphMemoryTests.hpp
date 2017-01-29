#ifndef GRAPHMEMORYTESTS_HPP_
#  define GRAPHMEMORYTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "GraphMemory.hpp"
#undef protected
#undef private

class GraphMemoryTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(GraphMemoryTests);
  CPPUNIT_TEST(test);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void test();
};

#endif /* GRAPHMEMORYTESTS_HPP_ */
