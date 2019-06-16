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

#include "CollectionTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(CollectionTests);

// A container of 2^2 elements by block with 0 block pre-allocated
static Collection<int, 2_z, Block> collection1(0);
// A container of 2^2 elements by block with 5/4 blocks pre-allocated
static Collection<int, 2_z, Block> collection2(5);
static Collection<int, 2_z, Block>::iterator it1;

#define INITIAL_INDEX  static_cast<size_t>(-1)

//--------------------------------------------------------------------------
void CollectionTests::setUp()
{
  std::cout << "*** CollectionTests ***************************************" << std::endl;
}

//--------------------------------------------------------------------------
void CollectionTests::tearDown()
{
}

//--------------------------------------------------------------------------
void CollectionTests::testDummy()
{
  // Check dummy container
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(true, collection1.full());
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);

  // Check dummy container
  CPPUNIT_ASSERT_EQUAL(0_z, collection2.used());
  CPPUNIT_ASSERT_EQUAL(2_z, collection2.blocks());
  CPPUNIT_ASSERT_EQUAL(8_z, collection2.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection2.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection2.full());
  CPPUNIT_ASSERT_EQUAL(false, collection2.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, collection2.outofbound(8U));
  CPPUNIT_ASSERT_EQUAL(false, collection2.occupied(0_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(8U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection2.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);

  // Useless operations
  collection1.remove(0_z);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(true, collection1.full());
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection2.occupied(0_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(8U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);

  // Useless operations
  CPPUNIT_ASSERT_THROW(collection1.modify(0, 42), std::out_of_range);

  // Useless operations
  collection1.garbage();
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(true, collection1.full());
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection2.occupied(0_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(8U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);

  // Useless operations
  collection1.clear();
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(true, collection1.full());
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection2.occupied(0_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(8U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);

  // Try occupy an empty block. Check this is not possible
  CPPUNIT_ASSERT_THROW(collection1.occupy(0), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
}

//--------------------------------------------------------------------------
void CollectionTests::testOccupy()
{
  // Try occupy an empty block. Check this is not possible
  /* collection1.insert(0, 41); collection1.clear();
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(0_z));
  collection1.occupy(1);
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(1_z));
  collection1.occupy(0);
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(1_z));

  // Clear
  collection1.clear();
  collection1.garbage();
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(true, collection1.full());
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection2.occupied(0_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(8U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);*/
}

//--------------------------------------------------------------------------
void CollectionTests::testAppend()
{
  collection1.insert(0, 41);
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(8U));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(41, collection1.get(0_z));
  CPPUNIT_ASSERT_THROW(collection1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.m_end);

  collection1.insert(1, 42);
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(2_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(41, collection1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, collection1.get(1_z));
  CPPUNIT_ASSERT_THROW(collection1.get(2_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.m_end);

  collection1.insert(2, 43);
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(2_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(3_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(41, collection1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, collection1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(43, collection1.get(2_z));
  CPPUNIT_ASSERT_THROW(collection1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.m_end);

  collection1.insert(3, 44);
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(true, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(2_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(3_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(41, collection1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, collection1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(43, collection1.get(2_z));
  CPPUNIT_ASSERT_EQUAL(44, collection1.get(3_z));
  CPPUNIT_ASSERT_THROW(collection1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.m_end);

  collection1.insert(4, 45);
  CPPUNIT_ASSERT_EQUAL(5_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(5_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(8U));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(2_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(4_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(5_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(8U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(41, collection1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, collection1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(43, collection1.get(2_z));
  CPPUNIT_ASSERT_EQUAL(44, collection1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(45, collection1.get(4_z));
  CPPUNIT_ASSERT_THROW(collection1.get(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(5_z, collection1.m_end);

  collection1.garbage();
  CPPUNIT_ASSERT_EQUAL(5_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(5_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(8U));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(2_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(4_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(5_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(8U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(41, collection1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, collection1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(43, collection1.get(2_z));
  CPPUNIT_ASSERT_EQUAL(44, collection1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(45, collection1.get(4_z));
  CPPUNIT_ASSERT_THROW(collection1.get(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(5_z, collection1.m_end);

  // Check iterators
  const int expected_values[5] =
    {
      41, 42, 43, 44, 45
    };

  int i = 0;
  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values[i], (*it1));
      ++i;
    }
}

//--------------------------------------------------------------------------
void CollectionTests::testRemove()
{
  collection1.remove(1_z);
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(41, collection1.get(0_z));
  CPPUNIT_ASSERT_THROW(collection1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(43, collection1.get(2_z));
  CPPUNIT_ASSERT_EQUAL(44, collection1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(45, collection1.get(4_z));
  CPPUNIT_ASSERT_THROW(collection1.get(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(5_z, collection1.m_end);

  // Check iterators
  const int expected_values1[4] =
    {
      41, 43, 44, 45
    };

  int i = 0;
  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values1[i], (*it1));
      ++i;
    }

  collection1.remove(3_z);
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(5_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(41, collection1.get(0_z));
  CPPUNIT_ASSERT_THROW(collection1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(43, collection1.get(2_z));
  CPPUNIT_ASSERT_THROW(collection1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(45, collection1.get(4_z));
  CPPUNIT_ASSERT_THROW(collection1.get(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(5_z, collection1.m_end);

  // Check iterators
  const int expected_values2[3] =
    {
      41, 43, 45
    };

  i = 0;
  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values2[i], (*it1));
      ++i;
    }

  collection1.remove(0_z);
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(6_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(3_z));
  CPPUNIT_ASSERT_THROW(collection1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(43, collection1.get(2_z));
  CPPUNIT_ASSERT_THROW(collection1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(45, collection1.get(4_z));
  CPPUNIT_ASSERT_THROW(collection1.get(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(5_z, collection1.m_end);

  // Check iterators
  const int expected_values3[2] =
    {
      43, 45
    };

  i = 0;
  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values3[i], (*it1));
      ++i;
    }

  collection1.remove(4_z);
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(7_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(4_z));
  CPPUNIT_ASSERT_THROW(collection1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(43, collection1.get(2_z));
  CPPUNIT_ASSERT_THROW(collection1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.m_end);

  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(43, (*it1));
    }

  collection1.remove(2_z);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(8_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(4_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(5_z));
  CPPUNIT_ASSERT_THROW(collection1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);

  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_FAIL("Not supposed to iterate");
    }

  collection1.remove(0_z);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(8_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(4_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(5_z));
  CPPUNIT_ASSERT_THROW(collection1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);

  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_FAIL("Not supposed to iterate");
    }

  collection1.garbage();
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(true, collection1.full());
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.outofbound(5_z));
  CPPUNIT_ASSERT_THROW(collection1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.occupied(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.occupied(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.occupied(5_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(collection1.get(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);

  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_FAIL("Not supposed to iterate");
    }
}

//--------------------------------------------------------------------------
void CollectionTests::testInsert()
{
  collection1.insert(3, 33);
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(33, collection1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.m_end);

  // Check iterators
  const int expected_values1[1] =
    {
      33
    };

  int i = 0;
  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values1[i], (*it1));
      ++i;
    }

  collection1.insert(15, 55);
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(14_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(15_z));
  CPPUNIT_ASSERT_EQUAL(33, collection1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(55, collection1.get(15_z));
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(16_z, collection1.m_end);

  // Check iterators
  const int expected_values2[2] =
    {
      33, 55
    };

  i = 0;
  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values2[i], (*it1));
      ++i;
    }

  collection1.insert(8, 88);
  collection1.insert(9, 99);
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(12_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(15_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(8U));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(9U));
  CPPUNIT_ASSERT_EQUAL(33, collection1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(55, collection1.get(15_z));
  CPPUNIT_ASSERT_EQUAL(88, collection1.get(8U));
  CPPUNIT_ASSERT_EQUAL(99, collection1.get(9U));
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(16_z, collection1.m_end);

  // Check iterators
  const int expected_values3[4] =
    {
      33, 88, 99, 55
    };

  i = 0;
  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values3[i], (*it1));
      ++i;
    }

  collection1.remove(8);
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(13_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(15_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(8U));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(9U));
  CPPUNIT_ASSERT_EQUAL(33, collection1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(55, collection1.get(15_z));
  CPPUNIT_ASSERT_EQUAL(99, collection1.get(9U));
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(16_z, collection1.m_end);

  // Check iterators
  const int expected_values4[3] =
    {
      33, 99, 55
    };

  i = 0;
  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values4[i], (*it1));
      ++i;
    }


  collection1.remove(15);
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(14_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(15_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(8U));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(9U));
  CPPUNIT_ASSERT_EQUAL(33, collection1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(99, collection1.get(9U));
  CPPUNIT_ASSERT_EQUAL(3_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(10_z, collection1.m_end);

  // Check iterators
  const int expected_values5[2] =
    {
      33, 99
    };

  i = 0;
  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values5[i], (*it1));
      ++i;
    }

  collection1.remove(3);
  CPPUNIT_ASSERT_EQUAL(1_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(15_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(15_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(8U));
  CPPUNIT_ASSERT_EQUAL(true, collection1.occupied(9U));
  CPPUNIT_ASSERT_EQUAL(99, collection1.get(9U));
  CPPUNIT_ASSERT_EQUAL(9_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(10_z, collection1.m_end);

  // Check iterators
  const int expected_values6[1] =
    {
      99
    };

  i = 0;
  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values6[i], (*it1));
      ++i;
    }

  collection1.remove(9);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(16_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(15_z));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(8U));
  CPPUNIT_ASSERT_EQUAL(false, collection1.occupied(9U));
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);

  for (it1 = collection1.begin(); it1 != collection1.end(); ++it1)
    {
      CPPUNIT_FAIL("Not supposed to iterate");
    }

  collection1.insert(8, 88);
  collection1.insert(9, 99);
  CPPUNIT_ASSERT_EQUAL(2_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(14_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(8_z, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(10_z, collection1.m_end);

  collection1.clear();
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(4_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(16_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(false, collection1.full());
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);

  collection1.garbage();
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, collection1.empty());
  CPPUNIT_ASSERT_EQUAL(true, collection1.full());
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, collection1.m_begin);
  CPPUNIT_ASSERT_EQUAL(0_z, collection1.m_end);
}
