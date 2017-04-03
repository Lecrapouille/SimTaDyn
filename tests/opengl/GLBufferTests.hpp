#ifndef GLBUFFERTESTS_HPP_
#  define GLBUFFERTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "GLBuffer.tpp"
#undef protected
#undef private

class GLBufferTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(GLBufferTests);
  CPPUNIT_TEST(testDummy);
  CPPUNIT_TEST(testPendingData);
  CPPUNIT_TEST(testInsert);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testDummy();
  void testPendingData();
  void testInsert();
};


#endif /* GLBUFFERTESTS_HPP_ */
