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

#ifndef TRANSFORMATIONTESTS_HPP_
#  define TRANSFORMATIONTESTS_HPP_

#  include <cppunit/TestFixture.h>
#  include <cppunit/TestResult.h>
#  include <cppunit/extensions/HelperMacros.h>

#  define protected public
#  define private public
#  include "Transformation.tpp"
#  include "Movable.tpp"
#  undef protected
#  undef private

class TransformationTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(TransformationTests);
  CPPUNIT_TEST(tests);
  CPPUNIT_TEST(movable);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void tests();
  void movable();
};

#endif
