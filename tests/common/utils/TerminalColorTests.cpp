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

#include "TerminalColorTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(TerminalColorTests);

//--------------------------------------------------------------------------
void TerminalColorTests::setUp()
{
}

//--------------------------------------------------------------------------
void TerminalColorTests::tearDown()
{
}

//--------------------------------------------------------------------------
void TerminalColorTests::testcolors()
{
  // Disable colorful text
  {
    std::stringstream ss;
    ss << termcolor::state::off << termcolor::fg::green << "Hello" << termcolor::style::normal << "world";
    CPPUNIT_ASSERT(ss.str() == "Helloworld");
  }

  // Enable colorful text
  {
    std::stringstream ss;
    ss << termcolor::state::on << termcolor::fg::green << "Hello" << termcolor::style::normal << "world";
    CPPUNIT_ASSERT(ss.str() == "\033[32mHello\033[0mworld");
  }

  // Disable colorful text
  {
    termcolor::disable();
    std::stringstream ss;
    ss << termcolor::fg::green << "Hello" << termcolor::bg::red << "world" << termcolor::style::normal;
    CPPUNIT_ASSERT(ss.str() == "Helloworld");
  }

  // Enable colorful text
  {
    termcolor::enable();
    std::stringstream ss;
    ss << termcolor::fg::green << "Hello" << termcolor::bg::red << "world" << termcolor::style::normal;
    CPPUNIT_ASSERT(ss.str() == "\033[32mHello\033[41mworld\033[0m");
  }

  // Disable colorful text
  {
    termcolor::disable();
    termcolor::color c1(termcolor::style::bold, termcolor::fg::red);
    termcolor::color c2(termcolor::style::italic, termcolor::fg::green);
    std::stringstream ss;
    ss << c1 << "Hello" << c2 << "world" << termcolor::style::normal;
    CPPUNIT_ASSERT(ss.str() == "Helloworld");
  }

  // Enable colorful text
  {
    termcolor::enable();
    termcolor::color c1(termcolor::style::bold, termcolor::fg::red);
    termcolor::color c2(termcolor::style::italic, termcolor::fg::green);
    std::stringstream ss;
    ss << c1 << "Hello" << c2 << "world" << termcolor::style::normal;
    CPPUNIT_ASSERT(ss.str() == "\033[1;31mHello\033[3;32mworld\033[0m");
  }
}
