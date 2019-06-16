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

#include "StrongTypeTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(StrongTypeTests);

//--------------------------------------------------------------------------
void StrongTypeTests::setUp()
{
  std::cout << "*** StrongTypeTests ***************************************" << std::endl;
}

//--------------------------------------------------------------------------
void StrongTypeTests::tearDown()
{
}

//--------------------------------------------------------------------------
void StrongTypeTests::testEquality()
{
  STRONG_TYPEDEF(int, Foo);
  STRONG_TYPEDEF(int, Bar);

  Foo f1;
  Foo f2(42);
  Bar f3;

  f1 = 42;
  f3 = 20;
  CPPUNIT_ASSERT_EQUAL(true, f1 == f2);
  CPPUNIT_ASSERT_EQUAL(true, f3 != Bar(42));
  CPPUNIT_ASSERT_EQUAL(true, f3 < Bar(42));
  CPPUNIT_ASSERT_EQUAL(true, Bar(42) <= Bar(42));
  CPPUNIT_ASSERT_EQUAL(true, Bar(42) >= Bar(42));
  CPPUNIT_ASSERT_EQUAL(true, Bar(42) == Bar(42));
  CPPUNIT_ASSERT_EQUAL(true, Bar(42) < Bar(43));
  CPPUNIT_ASSERT_EQUAL(true, Bar(43) > Bar(42));
}
