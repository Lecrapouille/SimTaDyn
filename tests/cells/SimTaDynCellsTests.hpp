#ifndef SIMTADYNCELLSTESTS_HPP_
#  define SIMTADYNCELLSTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "SimTaDynCells.hpp"
#undef protected
#undef private

class SimTaDynCellsTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(SimTaDynCellsTests);
  CPPUNIT_TEST(testCreator);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testCreator();
  void testID();
  void testNeighbors();
};

#endif /* SIMTADYNCELLSTESTS_HPP_ */
