//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

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
void FileTests::testfiles()
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

  res = File::extension("/home/qq/SimTaDyn/tests/common/FileTests.cpp~");
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

//--------------------------------------------------------------------------
void FileTests::testpath()
{
  Path path1;
  CPPUNIT_ASSERT_EQUAL(0, path1.toString().compare(""));
  Path path2("aa:bb:cc::ee::");
  std::cout << "Path: '" << path2.toString() << "'" << std::endl;
  path2.replace("/usr/bin/");
  std::cout << "Path: '" << path2.toString() << "'" << std::endl;
  CPPUNIT_ASSERT_EQUAL(0, path2.toString().compare("/usr/bin:"));
  path2.add("/usr/share:/bin/");
  std::cout << "Path: '" << path2.toString() << "'" << std::endl;
  CPPUNIT_ASSERT_EQUAL(0, path2.toString().compare("/usr/bin:/usr/share:/bin:"));
  std::pair<std::string, bool> res = path2.find("ls");
  CPPUNIT_ASSERT_EQUAL(true, res.second);
  CPPUNIT_ASSERT_EQUAL(0, res.first.compare("/bin/ls"));
}
