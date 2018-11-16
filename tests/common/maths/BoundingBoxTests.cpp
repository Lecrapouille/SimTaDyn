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

#include "BoundingBoxTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(BoundingBoxTests);

static AABB3f a1(Vector3f::ZERO, Vector3f::UNIT_SCALE);
static AABB3f a2(Vector3f(1), Vector3f(2,2,2));
static AABB3f a3 = AABB3f::UNIT_SCALE;
static AABB3f a4 = AABB3f::ZERO;
static AABB3f a5;
static AABB3f a6 = AABB3f::INFINITE;
static AABB3f a7 = AABB3f({1,1,1}, {2,2,2});
static AABB3f a8 = AABB3f::DUMMY;

static AABB2i i1;
static AABB2i i2(a3 * 4.0f);
static AABB2i i3(42);

//--------------------------------------------------------------------------
template <typename T, size_t n>
static inline void CHECK_VECTOR(Vector<T, n> const& v, const T x, const T y, const T z)
{
  CPPUNIT_ASSERT_EQUAL(x, v.x);
  CPPUNIT_ASSERT_EQUAL(y, v.y);
  CPPUNIT_ASSERT_EQUAL(z, v.z);
}

//--------------------------------------------------------------------------
template <typename T, size_t n>
static inline void checkBox(AABB<T, n> const &aabb, Vector<T, n> const &v1,
                            Vector<T, n> const &v2)
{
  CPPUNIT_ASSERT_EQUAL(true, vector::eq(v1, aabb.m_bbmin));
  CPPUNIT_ASSERT_EQUAL(true, vector::eq(v2, aabb.m_bbmax));
}

//--------------------------------------------------------------------------
template <typename T, size_t n>
static inline void checkBoxDummy(AABB<T, n> const &aabb)
{
  for (size_t i = 0_z; i < n; ++i)
    {
      CPPUNIT_ASSERT_EQUAL(true, std::isnan(aabb.m_bbmin[i]));
      CPPUNIT_ASSERT_EQUAL(true, std::isnan(aabb.m_bbmax[i]));
    }
}

//--------------------------------------------------------------------------
template <typename T, size_t n>
static bool isWellFormed(AABB<T, n> const &aabb)
{
  if (aabb.m_bbmin.size() != n)
    return false;

  size_t i = n;
  while (i--)
    {
      if (aabb.m_bbmin[i] > aabb.m_bbmax[i])
        {
          return false;
        }
    }
  return true;
}

//--------------------------------------------------------------------------
void BoundingBoxTests::setUp()
{
}

//--------------------------------------------------------------------------
void BoundingBoxTests::tearDown()
{
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testCreator()
{
  CPPUNIT_ASSERT_EQUAL(6_z * sizeof (float), sizeof (AABB3f));
  CPPUNIT_ASSERT_EQUAL(6_z * sizeof (int32_t), sizeof (AABB3i));
  CPPUNIT_ASSERT_EQUAL(6_z * sizeof (double), sizeof (AABB3g));

  // Display
  std::cout << "Displaying AABBs" << std::endl << a1 << a2 << a3
            << a4 << a5 << a6 << a7 << std::endl; // << i1 << i2 << i3

  // Check lower corner of the box is <= to the upper corner
  CPPUNIT_ASSERT_EQUAL(true, isWellFormed(a1));
  CPPUNIT_ASSERT_EQUAL(true, isWellFormed(a2));
  CPPUNIT_ASSERT_EQUAL(true, isWellFormed(a3));
  CPPUNIT_ASSERT_EQUAL(true, isWellFormed(a4));
  checkBoxDummy(a8);
  CPPUNIT_ASSERT_EQUAL(true, isWellFormed(a5));
  CPPUNIT_ASSERT_EQUAL(true, isWellFormed(a6));
  CPPUNIT_ASSERT_EQUAL(true, isWellFormed(a7));

  // Integer boxes
  CPPUNIT_ASSERT_EQUAL(true, isWellFormed(i1));
  CPPUNIT_ASSERT_EQUAL(true, isWellFormed(i2));
  CPPUNIT_ASSERT_EQUAL(true, isWellFormed(i3));
  checkBox(i2, Vector2i(-2, -2), Vector2i(2, 2));
  checkBox(i3, Vector2i(42, 42), Vector2i(42, 42));

  // Change dimension
  a5.set({1,1,1}, {2,2,2});
  CPPUNIT_ASSERT_EQUAL(true, eq(a5, a2));
  CPPUNIT_ASSERT_EQUAL(true, eq(a7, a2));

  // Getter
  CPPUNIT_ASSERT_EQUAL(true, vector::eq(a5.m_bbmin, a5.get(aabb::LowerCorner)));
  CPPUNIT_ASSERT_EQUAL(true, vector::eq(a5.m_bbmax, a5.get(aabb::UpperCorner)));
  CPPUNIT_ASSERT_EQUAL(true, vector::ne(a4.m_bbmin, a5.get(aabb::LowerCorner)));
  CPPUNIT_ASSERT_EQUAL(true, vector::ne(a4.m_bbmax, a5.get(aabb::UpperCorner)));

  // Try to malformed a AABB. Check security is triggered
  CPPUNIT_ASSERT_THROW(a5.set(Vector3f(2,2,2), Vector3f(1,1,1)), std::out_of_range);
  CPPUNIT_ASSERT_THROW(AABB3f({2,2,2}, {1,1,1}), std::out_of_range);
  CPPUNIT_ASSERT_THROW(AABB3f(Vector3f(2), Vector3f(1)), std::out_of_range);
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testSwap()
{
  swap(a1, a2);
  checkBox(a1, Vector3f(1,1,1), Vector3f(2,2,2));
  checkBox(a2, Vector3f::ZERO, Vector3f::UNIT_SCALE);

  swap(a1, a2);
  checkBox(a2, Vector3f(1,1,1), Vector3f(2,2,2));
  checkBox(a1, Vector3f::ZERO, Vector3f::UNIT_SCALE);

  swap(a2, a1);
  checkBox(a1, Vector3f(1,1,1), Vector3f(2,2,2));
  checkBox(a2, Vector3f::ZERO, Vector3f::UNIT_SCALE);

  swap(a2, a1);
  checkBox(a2, Vector3f(1,1,1), Vector3f(2,2,2));
  checkBox(a1, Vector3f::ZERO, Vector3f::UNIT_SCALE);
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testEquality()
{
  CPPUNIT_ASSERT_EQUAL(false, eq(a1, a2));
  CPPUNIT_ASSERT_EQUAL(true, ne(a1, a2));
  CPPUNIT_ASSERT_EQUAL(true, ne(a1, a2));
  CPPUNIT_ASSERT_EQUAL(true, ne(a1, a6));
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testArithmetic()
{
  AABB3f a = AABB3f({1,1,1}, {2,2,2});
  AABB3f b = AABB3f({-2,-2,-2}, {-1,-1,-1});

  checkBox(-a, Vector3f(-2), Vector3f(-1));
  checkBox(+(-a), Vector3f(-2), Vector3f(-1));
  checkBox(-b, Vector3f(1), Vector3f(2));
  checkBox(+(-b), Vector3f(1), Vector3f(2));
  checkBox(a + 1.0f, Vector3f(2,2,2), Vector3f(3,3,3));
  checkBox(a - 1.0f, Vector3f(0,0,0), Vector3f(1,1,1));
  checkBox(a * 2.0f, a.m_bbmin * 2.0f, a.m_bbmax * 2.0f);
  checkBox(a / 2.0f, a.m_bbmin / 2.0f, a.m_bbmax / 2.0f);

  // Suppressed because too dangerous: can create malformed AABB
  //Vector3f v = Vector3f(1,2,3);
  //checkBox(a + v, Vector3f(2,3,4), Vector3f(3,4,5));
  //checkBox(v + a, Vector3f(2,3,4), Vector3f(3,4,5));
  //checkBox(a - v, Vector3f(0,-1,-2), Vector3f(1,0,-1));
  //checkBox(v - a, Vector3f(0,1,2), Vector3f(-1,0,1));
  checkBox(1.0f + a, Vector3f(2), Vector3f(3));
  checkBox(1.0f - a, Vector3f(-1), Vector3f(0));
  checkBox(2.0f * a, a.m_bbmin * 2.0f, a.m_bbmax * 2.0f);
  checkBox(2.0f / a, 2.0f / a.m_bbmax, 2.0f / a.m_bbmin);

  // Suppressed because too dangerous: error of logic
  // Merge (and not the sum of box corners)
  //checkBox(a + a, a.m_bbmin, a.m_bbmax);
  // Union (and not the sum of box corners)
  //checkBox(a - a, a.m_bbmin, a.m_bbmax);
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testCopy()
{
  AABB3f old(a1);
  AABB3f tmp(old);

  a1 = a2;
  CPPUNIT_ASSERT_EQUAL((a1 == a2), true);
  a1 = AABB3f::UNIT_SCALE;
  CPPUNIT_ASSERT_EQUAL((a1 == a3), true);
  a1 = old;
  checkBox(a1, tmp.m_bbmin, tmp.m_bbmax);
}

//--------------------------------------------------------------------------
void BoundingBoxTests::testOperations()
{
  // Check the center
  CPPUNIT_ASSERT_EQUAL(true, vector::eq(Vector3f(1.5f, 1.5f, 1.5f), a2.center()));
  CPPUNIT_ASSERT_EQUAL(true, vector::eq(Vector3f(0), a4.center()));
  Vector3f c(AABB3f::DUMMY.center());
  for (size_t i = 0_z; i < 3_z; ++i)
    {
      CPPUNIT_ASSERT_EQUAL(true, std::isnan(c[i]));
    }

  // Check volume comparaison
  CPPUNIT_ASSERT_EQUAL(1.0f, a2.volume());
  CPPUNIT_ASSERT_EQUAL(4.0f * 4.0f * 4.0f, (a2 * 4.0f).volume());
  a5 =  a2 * 4.0f;
  CPPUNIT_ASSERT_EQUAL(true, (a5 > a2));
  CPPUNIT_ASSERT_EQUAL(false, (a5 < a2));
  CPPUNIT_ASSERT_EQUAL(true, (a2 >= a3));
  CPPUNIT_ASSERT_EQUAL(true, (a2 <= a3));
  CPPUNIT_ASSERT_EQUAL(true, (a2 == a3));
  CPPUNIT_ASSERT_EQUAL(true, (a6 > a3));

  // Check if a point is contained or not to an AABB.
  CPPUNIT_ASSERT_EQUAL(true, a2.contains(Vector3f(1.5f, 1.5f, 1.5f)));
  CPPUNIT_ASSERT_EQUAL(true, a2.contains(a2.center()));
  CPPUNIT_ASSERT_EQUAL(false, a2.contains(a2.center() * 3.0f));
  CPPUNIT_ASSERT_EQUAL(true, a6.contains(a2.center() * 3.0f));

  // Box containing others
  CPPUNIT_ASSERT_EQUAL(true, a1.contains(a1));
  CPPUNIT_ASSERT_EQUAL(false, a8.contains(a1));
  CPPUNIT_ASSERT_EQUAL(true, a6.contains(a1));
  CPPUNIT_ASSERT_EQUAL(false, AABB3f(0, 1).contains(AABB3f(1, 2))); // corner case

  // Clamp
  //checkBox(clamp(a1, a1.m_bbmin, a1.m_bbmax), a1.m_bbmin, a1.m_bbmax);
  //checkBox(clamp(a1 * 4.0f, a1.m_bbmin, a1.m_bbmax), a1.m_bbmin, a1.m_bbmax);

  // Box collisions
  CPPUNIT_ASSERT_EQUAL(false, a2.collides(a4));
  CPPUNIT_ASSERT_EQUAL(true, a2.collides(AABB3f(Vector3f(2,2,2), Vector3f(3,3,3))));
  CPPUNIT_ASSERT_EQUAL(true, a6.collides(a4));

  checkBox(intersection(a3, a3), a3.m_bbmin, a3.m_bbmax);
  checkBox(merge(a3, a3), a3.m_bbmin, a3.m_bbmax);
  checkBox(intersection(a3, a6), a3.m_bbmin, a3.m_bbmax);

  checkBox(merge(a6, a6), a6.m_bbmin, a6.m_bbmax);
  checkBox(merge(a6, a3), a6.m_bbmin, a6.m_bbmax);
  checkBox(merge(a3, a6), a6.m_bbmin, a6.m_bbmax);
  checkBox(intersection(a6, a3), a3.m_bbmin, a3.m_bbmax);
  checkBox(intersection(a3, a6), a3.m_bbmin, a3.m_bbmax);
  checkBox(merge(a2, a3), a3.m_bbmin, a2.m_bbmax);

  // Try intersection with disjoint AABB.
  checkBoxDummy(intersection(a2, a3));
  checkBoxDummy(intersection(a5, a3));

  // Check intersecation at an AABB corner.
  checkBox(intersection(a7, AABB3f(0, 1)), Vector3f(1), Vector3f(1));
  checkBox(intersection(AABB3f(0, 1), AABB3f(-1, 0)), Vector3f(0), Vector3f(0));

  // AABB collisions
  CPPUNIT_ASSERT_EQUAL(true, a4.collides(a4));
  CPPUNIT_ASSERT_EQUAL(false, a4.collides(a2));
  CPPUNIT_ASSERT_EQUAL(false, a2.collides(a4));
  CPPUNIT_ASSERT_EQUAL(true, a2.collides(AABB3f(Vector3f(2,2,2), Vector3f(3,3,3))));
  CPPUNIT_ASSERT_EQUAL(true, a6.collides(a4));
  CPPUNIT_ASSERT_EQUAL(true, a4.collides(a6));

  //

  CPPUNIT_ASSERT_EQUAL(std::numeric_limits<float>::infinity(), a6.volume());
  CPPUNIT_ASSERT_EQUAL(true, vector::eq(Vector3f(std::numeric_limits<float>::infinity()),
                                        a6.size()));
}
