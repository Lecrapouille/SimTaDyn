#ifndef BASICNODETESTS_HPP_
#  define BASICNODETESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "Graph.hpp"
#undef protected
#undef private

class BasicNodeTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(BasicNodeTests);
  CPPUNIT_TEST(dummy);
  CPPUNIT_TEST(neighbor);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void dummy();
  void neighbor();
};

#endif /* BASICNODETESTS_HPP_ */
