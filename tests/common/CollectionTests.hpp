#ifndef COLLECTIONTESTS_HPP_
#  define COLLECTIONTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "Collection.tpp"
#undef protected
#undef private

class CollectionTests : public CppUnit::TestFixture
{
  // CppUnit macros for collectionting up the test suite
  CPPUNIT_TEST_SUITE(CollectionTests);
  CPPUNIT_TEST(testDummy);
  CPPUNIT_TEST(testOccupy);
  CPPUNIT_TEST(testAppend);
  CPPUNIT_TEST(testRemove);
  CPPUNIT_TEST(testInsert);
  CPPUNIT_TEST_SUITE_END();

public:
  void collectionUp();
  void tearDown();

  void testDummy();
  void testOccupy();
  void testAppend();
  void testRemove();
  void testInsert();
};


#endif /* COLLECTIONTESTS_HPP_ */
