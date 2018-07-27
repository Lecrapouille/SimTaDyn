#include "Maths.hpp"

namespace maths
{
  //! \brief
  uint32_t maxUlps = 4U;
  bool fastSqrt = false;

  bool almostEqual(float const A, float const B)
  {
    if (A == B)
      return true;

    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    assert(maths::maxUlps < 4U * 1024U * 1024U);

    int aInt = *(int*)&A;

    // Make aInt lexicographically ordered as a twos-complement int
    if (aInt < 0)
      aInt = 0x80000000 - aInt;

    // Make bInt lexicographically ordered as a twos-complement int
    int bInt = *(int*)&B;

    if (bInt < 0)
      bInt = 0x80000000 - bInt;

    int intDiff = maths::abs(aInt - bInt);
    if (intDiff <= (int) maths::maxUlps)
        return true;
    return false;
  }

  bool almostZero(float const A)
  {
    return almostEqual(A, 0.0f);
  }

  float fastInvSqrt(const float number)
  {
#if 0
    long i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = number * 0.5f;
    y = number;
    i = * (long *) &y;
    i = 0x5f3759df - (i >> 1);
    y = * (float *) &i;
    // 1st iteration
    y = y * (threehalfs - (x2 * y * y));
    // 2nd iteration, this can be removed
    y = y * (threehalfs - (x2 * y * y));

    return y;
#else
    union
    {
      int32_t i;
      float   f;
    } conv;

    conv.f = number;
    conv.i = 0x5f3759df - (conv.i >> 1);
    return 0.5f * conv.f * (3.0f - number * conv.f * conv.f);
#endif
  }

};

#include "Maths.hpp"
