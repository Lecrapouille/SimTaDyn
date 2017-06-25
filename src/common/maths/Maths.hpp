#ifndef MATHS_HPP_
#  define MATHS_HPP_

#  include <cmath>
#  include <limits>
#  include <cassert>
#  include <iostream>
#  include <cstdint>

namespace maths
{
  //! \brief
  extern uint32_t maxUlps;
  extern bool fastSqrt;

  template< typename T >
  inline T sqrt(T const val)
  {
    return std::sqrt(val);
  }

  //! \brief Linear interpolation
  template< typename T >
  inline T lerp(T const from, T const to, float const f)
  {
    return from + ((to - from) * f);
  }

  //! \brief Linear interpolation
  template<>
  inline int lerp(int const from, int const to, float const f)
  {
    return (int) ((float) from + (((float) to - (float) from) * f));
  }

  //! \brief Absolute value
  template <typename T>
  inline T abs(T const val)
  {
    if (val < 0) return -val;
    return val;
  }

  //! \brief Constrain value: std::min(std::max(a[i], lower), upper)
  template <typename T>
  inline T clamp(T const value, T const lower, T const upper)
  {
    if (value < lower)
      return lower;

    if (value > upper)
      return upper;

    return value;
  }

  /*
  template <typename T>
  typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
  almostEqual(T const x, T const y)
  {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x-y) < std::numeric_limits<T>::epsilon() * std::abs(x+y) * maths::maxUlps
    // unless the result is subnormal
           || std::abs(x-y) < std::numeric_limits<T>::min();
  }
  */
  // http://bitbashing.io/comparing-floats.html
  // https://www.working-software.com/cpp-float-comparisons
  // http://www.cygnus-software.com/papers/comparingfloats/Comparing%20floating%20point%20numbers.htm#_Toc135149455
  // Usable AlmostEqual function
  //template <>

  bool almostEqual(float const A, float const B);
  bool almostZero(float const A);

}

#endif
