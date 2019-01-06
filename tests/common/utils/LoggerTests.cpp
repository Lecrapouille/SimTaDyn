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

#include "LoggerTests.hpp"
#include <thread>
#include <algorithm>
#include <iterator>
#include <cstring>

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(LoggerTests);

static const uint32_t header_footer_lines = 6U + 5U;

//--------------------------------------------------------------------------
void LoggerTests::setUp()
{
}

//--------------------------------------------------------------------------
void LoggerTests::tearDown()
{
}

//--------------------------------------------------------------------------
static uint32_t number_of_lines(std::string const& file)
{
  uint32_t line_count = 0U;
  std::ifstream myfile(file);
  if (!myfile)
    {
      std::cerr << "Could not open log '" << file << "' Reason: '"
                << strerror(errno) << "'" << std::endl;
      return 0u;
    }

  // New lines will be skipped unless we stop it from happening:
  myfile.unsetf(std::ios_base::skipws);

  // Count the newlines with an algorithm specialized for counting:
  line_count = std::count(
    std::istream_iterator<char>(myfile),
    std::istream_iterator<char>(), '\n');

  return line_count;
}

static void call_from_thread(uint32_t const x, uint32_t const lines_by_thread)
{
  for (uint32_t i = 0U; i < lines_by_thread; ++i)
    {
      LOGI("Hello World from thread %3u", x);
    }
}

//--------------------------------------------------------------------------
void LoggerTests::testEmptyLog()
  {
    constexpr uint32_t num_threads = 0U;
    constexpr uint32_t lines_by_thread = 0U;

    Logger::instance("/tmp/empty.log");
    Logger::destroy();

    uint32_t lines = number_of_lines("/tmp/empty.log");
    CPPUNIT_ASSERT_EQUAL(num_threads * lines_by_thread + header_footer_lines, lines);
  }

void LoggerTests::testBasic()
  {
    constexpr uint32_t num_threads = 1U;
    constexpr uint32_t lines_by_thread = 2U;

    // Change log with a full path
    CPPUNIT_ASSERT(Logger::instance().changeLog("/tmp/foo/bar/new.log"));
    CPP_LOG(logger::Fatal) << "test\n";
    LOGE("Coucou");
    Logger::destroy();
    uint32_t lines = number_of_lines("/tmp/foo/bar/new.log");
    CPPUNIT_ASSERT_EQUAL(num_threads * lines_by_thread + header_footer_lines, lines);

    // Chnage log with a simple file name
    CPPUNIT_ASSERT(Logger::instance().changeLog("simplefile.log"));
    CPP_LOG(logger::Warning) << "test2\n";
    LOGD("Coucou");
    LOGW("TEST");
    Logger::destroy();
    lines = number_of_lines(config::tmp_path + "simplefile.log");
    CPPUNIT_ASSERT_EQUAL(num_threads * (lines_by_thread + 1) + header_footer_lines, lines);
  }

void LoggerTests::testWithConcurrency()
  {
    constexpr uint32_t num_threads = 10U;
    constexpr uint32_t lines_by_thread = 10U;

    Logger::instance();

    static std::thread t[num_threads];

    // Launch a group of threads
    for (uint32_t i = 0; i < num_threads; ++i)
      {
        t[i] = std::thread(call_from_thread, i, lines_by_thread);
      }

    // Join the threads with the main thread
    for (uint32_t i = 0; i < num_threads; ++i)
      {
        if (t[i].joinable())
          {
            t[i].join();
          }
      }
    Logger::destroy();

    // Check: count the number of lines == num_threads + header + footer
    uint32_t lines = number_of_lines(config::log_path);
    CPPUNIT_ASSERT_EQUAL(num_threads * lines_by_thread + header_footer_lines, lines);
  }
