#ifndef FILETESTS_HPP_
#  define FILETESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "File.hpp"
#include "Path.hpp"
#undef protected
#undef private

class FileTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(FileTests);
  CPPUNIT_TEST(testfiles);
  CPPUNIT_TEST(testpath);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testfiles();
  void testpath();
};

#endif /* FILETESTS_HPP_ */
