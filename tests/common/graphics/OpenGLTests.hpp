#ifndef OPENGLTESTS_HPP_
#  define OPENGLTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "GLAttrib.hpp"
#include "GLVertexArray.hpp"
#undef protected
#undef private

class OpenGLTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(OpenGLTests);
  CPPUNIT_TEST(testOpenGL);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void testOpenGL();
};

#endif
