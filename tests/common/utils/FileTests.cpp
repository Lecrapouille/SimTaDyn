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

  res = File::fileName("/home/qq/SimTaDyn/tests/common/FileTests.cpp");
  CPPUNIT_ASSERT_EQUAL(true, "FileTests.cpp" == res);

  res = File::fileName("/home/qq/SimTaDyn/tests/common");
  CPPUNIT_ASSERT_EQUAL(true, "common" == res);

  res = File::fileName("common");
  CPPUNIT_ASSERT_EQUAL(true, "common" == res);

  res = File::baseName("/home/qq/SimTaDyn/tests/common/FileTests.cpp");
  CPPUNIT_ASSERT_EQUAL(true, "FileTests" == res);

  res = File::baseName("/home/qq/SimTaDyn/tests/common");
  CPPUNIT_ASSERT_EQUAL(true, "common" == res);

  res = File::baseName("common");
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

  CPPUNIT_ASSERT_EQUAL(false, File::exist("pouet"));
  CPPUNIT_ASSERT_EQUAL(true, File::exist("/dev/null"));

  CPPUNIT_ASSERT(File::Directory == File::type("/tmp"));
  CPPUNIT_ASSERT(true == File::isReadable("/tmp"));
  CPPUNIT_ASSERT(true == File::isWritable("/tmp"));

  CPPUNIT_ASSERT(File::Directory == File::type("/usr/sbin/"));
  CPPUNIT_ASSERT(true == File::isReadable("/usr/sbin/"));
  CPPUNIT_ASSERT(false == File::isWritable("/usr/sbin/"));

  CPPUNIT_ASSERT(File::DoesNotExist == File::type("/usr/sbin/ls"));
  CPPUNIT_ASSERT(false == File::isReadable("/usr/sbin/ls"));
  CPPUNIT_ASSERT(false == File::isWritable("/usr/sbin/ls"));

  CPPUNIT_ASSERT(File::Document == File::type("/bin/ls"));
  CPPUNIT_ASSERT(true == File::isReadable("/bin/ls"));
  CPPUNIT_ASSERT(false == File::isWritable("/bin/ls"));

  CPPUNIT_ASSERT(true == File::mkdir("/tmp/qq"));
  CPPUNIT_ASSERT(true == File::mkdir("/tmp/qq/"));

  CPPUNIT_ASSERT(File::dirName("/tmp/foo/bar") == "/tmp/foo/");
  CPPUNIT_ASSERT(File::dirName("/tmp/foo/") == "/tmp/foo/");
  CPPUNIT_ASSERT(File::dirName("/tmp/foo") == "/tmp/");
  CPPUNIT_ASSERT(File::dirName("/tmp/") == "/tmp/");
  CPPUNIT_ASSERT(File::dirName("/tmp") == "/");
}

//--------------------------------------------------------------------------
void FileTests::testpath()
{
  Path path1;
  CPPUNIT_ASSERT_EQUAL(0, path1.toString().compare(""));
  Path path2("aa:bb:cc::ee::");
  std::cout << "Path: '" << path2.toString() << "'" << std::endl;
  path2.init("/usr/bin/");
  std::cout << "Path: '" << path2.toString() << "'" << std::endl;
  CPPUNIT_ASSERT_EQUAL(0, path2.toString().compare("/usr/bin:"));
  path2.add("/usr/share:/bin/");
  std::cout << "Path: '" << path2.toString() << "'" << std::endl;
  CPPUNIT_ASSERT_EQUAL(0, path2.toString().compare("/usr/bin:/usr/share:/bin:"));
  std::pair<std::string, bool> res = path2.find("ls");
  CPPUNIT_ASSERT_EQUAL(true, res.second);
  CPPUNIT_ASSERT_EQUAL(0, res.first.compare("/bin/ls"));
}
