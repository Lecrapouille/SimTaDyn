#ifndef FILETESTS_HPP_
#  define FILETESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "File.hpp"
#undef protected
#undef private

class FileTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(FileTests);
  CPPUNIT_TEST(tests);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void tests();
};

#endif /* FILETESTS_HPP_ */
