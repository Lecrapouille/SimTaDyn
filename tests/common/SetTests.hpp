#ifndef SETTESTS_HPP_
#  define SETTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "Set.tpp"
#undef protected
#undef private

class SetTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(SetTests);
  CPPUNIT_TEST(testDummy);
  CPPUNIT_TEST(testInsert);
  CPPUNIT_TEST(testRemove);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testDummy();
  void testInsert();
  void testRemove();
};


#endif /* SETTESTS_HPP_ */
