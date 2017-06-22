#ifndef FILTERINGTESTS_HPP_
#  define FILTERINGTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "Filtering.hpp"
#undef protected
#undef private

class FilteringTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(FilteringTests);
  CPPUNIT_TEST(rolling);
  CPPUNIT_TEST(lpf);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp();
  void tearDown();
  void rolling();
  void lpf();
};

#endif /* FILTERINGTESTS_HPP_ */
