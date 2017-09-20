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

#include "SimTaDynFileLoaderTests.hpp"
#include <iostream>
#include <fstream>

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(LoaderTests);

//--------------------------------------------------------------------------
void LoaderTests::setUp()
{
}

//--------------------------------------------------------------------------
void LoaderTests::tearDown()
{
}

//--------------------------------------------------------------------------
void LoaderTests::testSimTaDyn()
{
  Path path(SIMTADYN_DATA_PATH);
  SimTaDynFileLoader loader;
  SimTaDynMap *map = nullptr;

  CPPUNIT_ASSERT_THROW(loader.unzip("thisfiledoesnotexist.zip"), LoaderException);

  loader.m_base_dir = "foo";
  std::string dir = loader.generateTempDirName();
  loader.unzip(path.expand("maps/MaCarte.dyn"));
  CPPUNIT_ASSERT_EQUAL(0, dir.compare(loader.m_base_dir));

  SimForth::instance().boot();
  dir = loader.generateTempDirName();
  loader.loadFromFile(path.expand("maps/MaCarte.dyn"), map);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != map);
  CPPUNIT_ASSERT_EQUAL(0, map->m_base_dir.compare(dir));
  CPPUNIT_ASSERT_EQUAL(0, map->m_name.compare("MaCarte"));
  std::cout << map->m_full_path << std::endl;

  /*std::ofstream outfile(loader.m_full_path + "/test.txt");
  outfile << "my text here!" << std::endl;
  outfile.close();

  loader.zip();
  CPPUNIT_ASSERT_EQUAL();*/

  SimForth::destroy();
}
