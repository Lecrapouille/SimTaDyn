#ifndef RESOURCES_TESTS_HPP_
#  define RESOURCES_TESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "ResourceManager.tpp"
#include "ILoader.hpp"
#undef protected
#undef private

class ResourcesTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(ResourcesTests);
  CPPUNIT_TEST(testsResources);
  CPPUNIT_TEST(testsResourceManager);
  CPPUNIT_TEST(testsLoaderManager);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp();
  void tearDown();
  void testsResources();
  void testsResourceManager();
  void testsLoaderManager();
};

#endif /* RESOURCES_TESTS_HPP_ */
