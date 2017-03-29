#include "FileTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(FileTests);

//--------------------------------------------------------------------------
void FileTests::setUp()
{
}

//--------------------------------------------------------------------------
void FileTests::tearDown()
{
}

//--------------------------------------------------------------------------
void FileTests::tests()
{
  std::string res;
  bool b;

  res = File::shortNameWithExtension("/home/qq/SimTaDyn/tests/common/FileTests.cpp");
  CPPUNIT_ASSERT_EQUAL(true, "FileTests.cpp" == res);

  res = File::shortNameWithExtension("/home/qq/SimTaDyn/tests/common");
  CPPUNIT_ASSERT_EQUAL(true, "common" == res);

  res = File::shortNameWithExtension("common");
  CPPUNIT_ASSERT_EQUAL(true, "common" == res);

  res = File::shortName("/home/qq/SimTaDyn/tests/common/FileTests.cpp");
  CPPUNIT_ASSERT_EQUAL(true, "FileTests" == res);

  res = File::shortName("/home/qq/SimTaDyn/tests/common");
  CPPUNIT_ASSERT_EQUAL(true, "common" == res);

  res = File::shortName("common");
  CPPUNIT_ASSERT_EQUAL(true, "common" == res);

  res = File::extension("/home/qq/SimTaDyn/tests/common/FileTests.cpp");
  CPPUNIT_ASSERT_EQUAL(true, "cpp" == res);

  res = File::extension("/home/qq/SimTaDyn/tests/common/FileTests.cpp.hpp");
  CPPUNIT_ASSERT_EQUAL(true, "hpp" == res);

  res = File::extension("/home/qq/SimTaDyn/tests/common");
  CPPUNIT_ASSERT_EQUAL(true, "" == res);

  res = File::extension("common");
  CPPUNIT_ASSERT_EQUAL(true, "" == res);

  b = File::exist("pouet");
  CPPUNIT_ASSERT_EQUAL(false, b);

  b = File::exist("/dev/null");
  CPPUNIT_ASSERT_EQUAL(true, b);

}
