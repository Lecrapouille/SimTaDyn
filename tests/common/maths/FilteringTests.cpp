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

#include "FilteringTests.hpp"
#include <iostream>
#include <iomanip>

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(FilteringTests);

//--------------------------------------------------------------------------
// Generated file by Scilab tool: input output
static double c_LPF[] = {
#include "generated/LowPassFilter1storder.h"
};
static double c_rolling[] = {
#include "generated/RollingAverageFilter.h"
};
static double c_polyfit_io[] = {
#include "generated/PolyFitIO.h"
};
static Vector4g c_polynom = {
#include "generated/PolyFit.h"
};

//--------------------------------------------------------------------------
// Return the number of elements in an array
#ifndef ARRAY_SIZE
#  define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#endif

//--------------------------------------------------------------------------
void FilteringTests::setUp()
{
  std::cout << "*** FilteringTests ***************************************" << std::endl;
}

//--------------------------------------------------------------------------
void FilteringTests::tearDown()
{
}

//--------------------------------------------------------------------------
void FilteringTests::rolling()
{
  const uint32_t N = ARRAY_SIZE(c_rolling) / 2U;
  CPPUNIT_ASSERT_EQUAL(126U, N);

  RollingAverageFilter filter1;
  filter1.configure(64U, 128U);
  for (uint32_t i = 0; i < N; ++i)
    {
      float U = c_rolling[2U * i + 0U]; // input
      float Y = c_rolling[2U * i + 1U]; // expected output
      float res = filter1.process(U);
      float error = fabs(Y - res);
      std::stringstream msg;
      msg << "Iter: " << i << " Input: " << U << " Expected: " << Y << " Obtained: " << res << std::endl;
      std::cout << msg.str() << std::endl;
      CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str(), true, error < 0.0001f);
    }
}

//--------------------------------------------------------------------------
void FilteringTests::lpf()
{
  // Filter a sinus. Compare results with Scicoslab +/- an error.
  LowPassFilter1stOrder filter1;
  const float init_val = 0.0f;
  filter1.configure(2.0f, 1.0f / 200.0f, init_val);


  const uint32_t N = ARRAY_SIZE(c_LPF) / 2U;
  CPPUNIT_ASSERT_EQUAL(126U, N);
  for (uint32_t i = 0; i < N; ++i)
    {
      float U = c_LPF[2U * i + 0U]; // input
      float Y = c_LPF[2U * i + 1U]; // expected output
      float res = filter1.process(U);
      float error;
      if (0U == i)
        {
          CPPUNIT_ASSERT_EQUAL(true, init_val == filter1.m_init_value);
          error = fabs(res - filter1.m_init_value);
        }
      else
        {
          error = fabs(Y - res);
        }
      std::stringstream msg;
      msg << "Iter: " << i << " Input: " << U << " Expected: " << Y << " Obtained: " << res << std::endl;
      //std::cout << std::fixed << U << ' ' << Y << ' ' << res << std::endl;
      CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str(), true, error < 1.0f);
    }

  // Low pass filter without calling the configure. Check the filter let pass the signal.
  LowPassFilter1stOrder filter2;
  CPPUNIT_ASSERT_EQUAL(true, 0.0f == filter2.process(c_LPF[0]));
  for (uint32_t i = 0U; i < N; ++i)
    {
      float U = c_LPF[2U * i + 0U]; // input
      float Y = c_LPF[2U * i + 0U]; // expected output
      float res = filter2.process(U);
      float error = fabs(Y - res);
      std::stringstream msg;
      msg << "Iter: " << i << " Input: " << U << " Expected: " << Y << " Obtained: " << res << std::endl;
      std::cout << msg.str() << std::endl;
      CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str(), true, error < 0.0001f);
    }
}

//--------------------------------------------------------------------------
static inline void checkAlmostVectorEps(Vector4g const& a, Vector4g const& b)
{
  CPPUNIT_ASSERT_EQUAL(true, maths::abs(a.x - b.x) < 0.000001);
  CPPUNIT_ASSERT_EQUAL(true, maths::abs(a.y - b.y) < 0.000001);
  CPPUNIT_ASSERT_EQUAL(true, maths::abs(a.z - b.z) < 0.000001);
  CPPUNIT_ASSERT_EQUAL(true, maths::abs(a.w - b.w) < 0.000001);
}

//--------------------------------------------------------------------------
void FilteringTests::polyfit()
{
  PolyFit<4U> fit;

  const uint32_t N = ARRAY_SIZE(c_polyfit_io) / 2U;
  CPPUNIT_ASSERT_EQUAL(10U, N);
  for (uint32_t i = 0; i < N; ++i)
    {
      double X = c_polyfit_io[2U * i + 0U]; // input
      double Y = c_polyfit_io[2U * i + 1U]; // output
      fit.process(X, Y);
    }
  Vector<double, 4U> obt_p = fit.end();
  checkAlmostVectorEps(obt_p, c_polynom);
}
