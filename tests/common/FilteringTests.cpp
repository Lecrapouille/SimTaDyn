#include "FilteringTests.hpp"
#include <iostream>
#include <iomanip>

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(FilteringTests);

//--------------------------------------------------------------------------
// Generated file by Scilab tool: input output
static float c_LPF[] = {
#include "generated/LowPassFilter1storder.h"
};
static float c_rolling[] = {
#include "generated/RollingAverageFilter.h"
};

//--------------------------------------------------------------------------
// Return the number of elements in an array
#ifndef ARRAY_SIZE
#  define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#endif

//--------------------------------------------------------------------------
static float fabs(const float v)
{
  if (v < 0.0f) return -v;
  return v;
}

//--------------------------------------------------------------------------
void FilteringTests::setUp()
{
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
