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

};

#include "Maths.hpp"
