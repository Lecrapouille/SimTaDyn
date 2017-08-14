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

#include "LoggerTests.hpp"
#include <thread>
#include <algorithm>
#include <iterator>

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(LoggerTests);

//--------------------------------------------------------------------------
void LoggerTests::setUp()
{
}

//--------------------------------------------------------------------------
void LoggerTests::tearDown()
{
}

//--------------------------------------------------------------------------
static uint32_t number_of_lines()
{
  uint32_t line_count = 0U;
  std::ifstream myfile("SimTaDyn.log");

  // new lines will be skipped unless we stop it from happening:
  myfile.unsetf(std::ios_base::skipws);

  // count the newlines with an algorithm specialized for counting:
  line_count = std::count(
    std::istream_iterator<char>(myfile),
    std::istream_iterator<char>(),
    '\n');

  return line_count;
}

//--------------------------------------------------------------------------
static const uint32_t num_threads = 1000;
static void call_from_thread(uint32_t const x)
{
  LOGI("Hello World from thread %3u", x);
}

//--------------------------------------------------------------------------
void LoggerTests::testlog()
{
  Logger::instance();
  std::thread t[num_threads];

  // Launch a group of threads
  for (uint32_t i = 0; i < num_threads; ++i)
    {
      t[i] = std::thread(call_from_thread, i);
    }

  // Join the threads with the main thread
  for (uint32_t i = 0; i < num_threads; ++i)
    {
      t[i].join();
    }
  Logger::destroy();

  // Check: count the number of lines == num_threads + header + footer
  uint32_t lines = number_of_lines();
  const uint32_t header_footer_lines = 4U + 5U;
  CPPUNIT_ASSERT_EQUAL(num_threads + header_footer_lines, lines);
}
