//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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

  CPPUNIT_ASSERT_EQUAL(true, File::Directory == File::type("/tmp"));
  CPPUNIT_ASSERT_EQUAL(true, File::isReadable("/tmp"));
  CPPUNIT_ASSERT_EQUAL(true, File::isWritable("/tmp"));

  if (File::exist("/usr/sbin/")) // FIXME: does not exist on Travis-CI docker
    {
      CPPUNIT_ASSERT_EQUAL(true, File::Directory == File::type("/usr/sbin/"));
      CPPUNIT_ASSERT_EQUAL(true, File::isReadable("/usr/sbin/"));
      //FIXME CPPUNIT_ASSERT_EQUAL(true, false == File::isWritable("/usr/sbin/"));
    }
  CPPUNIT_ASSERT_EQUAL(true, File::DoesNotExist == File::type("/usr/sbin/foobar"));
  CPPUNIT_ASSERT_EQUAL(false, File::isReadable("/usr/sbin/foobar"));
  CPPUNIT_ASSERT_EQUAL(false, File::isWritable("/usr/sbin/foobar"));

  CPPUNIT_ASSERT_EQUAL(true, File::Document == File::type("/bin/ls"));
  CPPUNIT_ASSERT_EQUAL(true, File::isReadable("/bin/ls"));
  //FIXME CPPUNIT_ASSERT_EQUAL(true, false == File::isWritable("/bin/ls"));

  CPPUNIT_ASSERT_EQUAL(true, File::mkdir("/tmp/qq"));
  CPPUNIT_ASSERT_EQUAL(true, File::mkdir("/tmp/foo/bar/"));
  CPPUNIT_ASSERT_EQUAL(true, File::mkdir("/tmp/foo/bar/"));

  CPPUNIT_ASSERT_EQUAL(false, File::mkdir("/dev/null"));

  CPPUNIT_ASSERT_EQUAL(true, File::dirName("/tmp/foo/bar") == "/tmp/foo/");
  CPPUNIT_ASSERT_EQUAL(true, File::dirName("/tmp/foo/") == "/tmp/foo/");
  CPPUNIT_ASSERT_EQUAL(true, File::dirName("/tmp/foo") == "/tmp/");
  CPPUNIT_ASSERT_EQUAL(true, File::dirName("/tmp/") == "/tmp/");
  CPPUNIT_ASSERT_EQUAL(true, File::dirName("/tmp") == "/");

  CPPUNIT_ASSERT_EQUAL(true, File::dirName("tmp/foo") == "tmp/");
  CPPUNIT_ASSERT_EQUAL(true, File::dirName("tmp/foo/") == "tmp/foo/");
  CPPUNIT_ASSERT_EQUAL(true, File::dirName("tmp") == "");
}
