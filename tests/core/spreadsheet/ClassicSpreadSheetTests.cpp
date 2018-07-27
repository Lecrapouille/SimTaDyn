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

#include "ClassicSpreadSheetTests.hpp"
#include "PathManager.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(ClassicSpreadSheetTests);

//--------------------------------------------------------------------------
void ClassicSpreadSheetTests::setUp()
{
}

//--------------------------------------------------------------------------
void ClassicSpreadSheetTests::tearDown()
{
}

static void eatSpreadsheet(ClassicSpreadSheet& sheet,
                           std::string const& file,
                           std::pair<bool, std::string> expected)
{
  std::cout << "Test spreadsheet '" << file << "'" << std::endl;

  // Get full path of the file
  std::cout << "PM: " << PathManager::instance().toString() << std::endl;
  std::string filename = PathManager::instance().expand(
             "../src/core/standalone/ClassicSpreadSheet/examples/" + file);

  // Start Forth
  SimForth& forth = SimForth::instance();
  forth.boot();

  // Compute thes spreadsheet
  CPPUNIT_ASSERT_MESSAGE(filename, sheet.readInput(filename));
  sheet.parse(forth);
  std::pair<bool, std::string> res = sheet.evaluate(forth);
  forth.ok(res);

  CPPUNIT_ASSERT_EQUAL(expected.first, res.first);
  CPPUNIT_ASSERT_EQUAL(expected.second, res.second);
}


//--------------------------------------------------------------------------
void ClassicSpreadSheetTests::testInput1()
{
  ClassicSpreadSheet sheet("Sheet1");
  eatSpreadsheet(sheet, "input1.txt", std::make_pair(true, "ok"));
  CPPUNIT_ASSERT(std::make_pair(true, 20) == sheet.value(0, 0));
  CPPUNIT_ASSERT(std::make_pair(true, 20) == sheet.value(0, 1));
  CPPUNIT_ASSERT(std::make_pair(true, 20) == sheet.value(0, 2));
  CPPUNIT_ASSERT(std::make_pair(true, 8) == sheet.value(1, 0));
  CPPUNIT_ASSERT(std::make_pair(true, 3) == sheet.value(1, 1));
  CPPUNIT_ASSERT(std::make_pair(true, 1) == sheet.value(1, 2));
}

void ClassicSpreadSheetTests::testInput2()
{
  ClassicSpreadSheet sheet("Sheet2");
  eatSpreadsheet(sheet, "input2.txt",
                 std::make_pair(false, "Aborting 'CircularDependencyFound: Unable to solve the spreadsheet'"));
  CPPUNIT_ASSERT_EQUAL(false, sheet.value(0, 0).first);
  CPPUNIT_ASSERT_EQUAL(false, sheet.value(0, 1).first);
  CPPUNIT_ASSERT_EQUAL(false, sheet.value(0, 2).first);
  CPPUNIT_ASSERT_EQUAL(false, sheet.value(1, 0).first);
  CPPUNIT_ASSERT(std::make_pair(true, 3) == sheet.value(1, 1));
  CPPUNIT_ASSERT_EQUAL(false, sheet.value(1, 2).first);
}

void ClassicSpreadSheetTests::testInput3()
{
  ClassicSpreadSheet sheet("Sheet3");
  eatSpreadsheet(sheet, "input3.txt",
                 std::make_pair(false, "Aborting 'CircularDependencyFound: Unable to solve the spreadsheet'"));
  CPPUNIT_ASSERT(std::make_pair(true, 6)  == sheet.value(0, 0));
  CPPUNIT_ASSERT_EQUAL(false,                sheet.value(0, 1).first);
  CPPUNIT_ASSERT(std::make_pair(true, 3)  == sheet.value(0, 2));
  CPPUNIT_ASSERT(std::make_pair(true, 12) == sheet.value(1, 0));
  CPPUNIT_ASSERT(std::make_pair(true, 6)  == sheet.value(1, 1));
  CPPUNIT_ASSERT(std::make_pair(true, 9)  == sheet.value(1, 2));
  CPPUNIT_ASSERT(std::make_pair(true, 15) == sheet.value(2, 0));
  CPPUNIT_ASSERT(std::make_pair(true, 10) == sheet.value(2, 1));
  CPPUNIT_ASSERT(std::make_pair(true, 4)  == sheet.value(2, 2));
}

void ClassicSpreadSheetTests::testInput4()
{
  ClassicSpreadSheet sheet("Sheet4");
  eatSpreadsheet(sheet, "input4.txt", std::make_pair(true, "ok"));
  CPPUNIT_ASSERT(std::make_pair(true, 6)   == sheet.value(0, 0));
  CPPUNIT_ASSERT(std::make_pair(true, -11) == sheet.value(0, 1));
  CPPUNIT_ASSERT(std::make_pair(true, 3)   == sheet.value(0, 2));
  CPPUNIT_ASSERT(std::make_pair(true, 12)  == sheet.value(1, 0));
  CPPUNIT_ASSERT(std::make_pair(true, 6)   == sheet.value(1, 1));
  CPPUNIT_ASSERT(std::make_pair(true, 9)   == sheet.value(1, 2));
  CPPUNIT_ASSERT(std::make_pair(true, 15)  == sheet.value(2, 0));
  CPPUNIT_ASSERT(std::make_pair(true, 10)  == sheet.value(2, 1));
  CPPUNIT_ASSERT(std::make_pair(true, 4)   == sheet.value(2, 2));
}

void ClassicSpreadSheetTests::testInput5()
{
  ClassicSpreadSheet sheet("Sheet5");
  eatSpreadsheet(sheet, "input5.txt", std::make_pair(true, "ok"));
  CPPUNIT_ASSERT(std::make_pair(true, -6) == sheet.value(0, 0));
  CPPUNIT_ASSERT(std::make_pair(true, 1)  == sheet.value(0, 1));
  CPPUNIT_ASSERT(std::make_pair(true, 3)  == sheet.value(0, 2));
  CPPUNIT_ASSERT(std::make_pair(true, 0)  == sheet.value(1, 0));
  CPPUNIT_ASSERT(std::make_pair(true, 6)  == sheet.value(1, 1));
  CPPUNIT_ASSERT(std::make_pair(true, 9)  == sheet.value(1, 2));
  CPPUNIT_ASSERT(std::make_pair(true, 3)  == sheet.value(2, 0));
  CPPUNIT_ASSERT(std::make_pair(true, 10) == sheet.value(2, 1));
  CPPUNIT_ASSERT(std::make_pair(true, 4)  == sheet.value(2, 2));
}
