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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "PathTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(PathTests);

//--------------------------------------------------------------------------
void PathTests::setUp()
{
  std::cout << "*** PathTests ***************************************" << std::endl;
}

//--------------------------------------------------------------------------
void PathTests::tearDown()
{
}

void PathTests::testEmptyConstructor()
{
  Path path;

  CPPUNIT_ASSERT(0 == path.m_search_paths.size());
  CPPUNIT_ASSERT_EQUAL(':', path.m_delimiter);
  CPPUNIT_ASSERT_EQUAL(true, path.m_string_path == "");
}

void PathTests::testSplitConstructor()
{
  Path path("/a/b:c/d");

  CPPUNIT_ASSERT(2 == path.m_search_paths.size());
  CPPUNIT_ASSERT_EQUAL(true, path.toString() == ".:/a/b:c/d:");
  path.clear();
  CPPUNIT_ASSERT(0 == path.m_search_paths.size());
  CPPUNIT_ASSERT_EQUAL(true, path.toString() == "");
  path.add("g/g");
  CPPUNIT_ASSERT(1 == path.m_search_paths.size());
  CPPUNIT_ASSERT_EQUAL(true, path.toString() == ".:g/g:");
  path.reset("a/b");
  CPPUNIT_ASSERT(1 == path.m_search_paths.size());
  CPPUNIT_ASSERT_EQUAL(true, path.toString() == ".:a/b:");
  std::cout << path.toString() << std::endl;
}

void PathTests::testSplitDir()
{
  Path path;
  path.split("/a//b\\d/:e\\d:");
  CPPUNIT_ASSERT_EQUAL(true, path.toString() == ".:/a//b\\d:e\\d:");
  CPPUNIT_ASSERT(2 == path.m_search_paths.size());
  path.remove("incorrect/path");
  CPPUNIT_ASSERT(2 == path.m_search_paths.size());
  path.remove("/a//b\\d");
  CPPUNIT_ASSERT(2 == path.m_search_paths.size()); // FIXME should be1
  path.remove("/a//b\\d/");
  CPPUNIT_ASSERT(1 == path.m_search_paths.size());
  CPPUNIT_ASSERT_EQUAL(true, path.toString() == ".:e\\d:");
  path.remove("e\\d/");
  CPPUNIT_ASSERT(0 == path.m_search_paths.size());
  path.remove("");
  CPPUNIT_ASSERT(0 == path.m_search_paths.size());
  path.remove("incorrect/path");
  CPPUNIT_ASSERT(0 == path.m_search_paths.size());
  path.add("g/g");
  CPPUNIT_ASSERT(1 == path.m_search_paths.size());
  CPPUNIT_ASSERT_EQUAL(true, path.toString() == ".:g/g:");
}

void PathTests::testFindAndExpand()
{
  Path path("/bin:/usr/bin:/usr/local/bin");
  CPPUNIT_ASSERT_EQUAL(true, path.expand("ls") != "ls");
  auto res1 = path.find("ls");

  CPPUNIT_ASSERT_EQUAL(true, res1.second);
  CPPUNIT_ASSERT_EQUAL(true, res1.first != "ls");
  CPPUNIT_ASSERT_EQUAL(true, res1.first != "");

  auto res3 = path.find(res1.first);
  CPPUNIT_ASSERT_EQUAL(true, res3.second);
  CPPUNIT_ASSERT_EQUAL(true, res3.first == res1.first);

  path.clear();
  CPPUNIT_ASSERT_EQUAL(true, path.expand("ls") == "ls");
  auto res2 = path.find("ls");
  CPPUNIT_ASSERT_EQUAL(false, res2.second);
  CPPUNIT_ASSERT_EQUAL(true, res2.first == "");
}
