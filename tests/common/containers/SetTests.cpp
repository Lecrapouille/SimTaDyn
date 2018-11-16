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

#include "SetTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(SetTests);

// A container of 2^2 elements by block with 0 block pre-allocated
static Set<int, 2_z, Block> set1(0);
// A container of 2^2 elements by block with 5/4 blocks pre-allocated
static Set<int, 2_z, Block> set2(5);
// A container of 2^2 elements by block with 0 block pre-allocated
static Set<int, 2_z, Block> set3(0);
// A container of 2^4 elements by block with 0 block pre-allocated
static Set<int, 4_z, Block> set4(0);
// A container of 2^5 elements by block with 0 block pre-allocated
static Set<int, 5_z, Block> set5(0);
// A container of 2^6 elements by block with 0 block pre-allocated
static Set<int, 6_z, Block> set6(0);
// A container of 2^16 elements by block with 0 block pre-allocated
static Set<int, 16_z, Block> set7(0);
static Set<int, 2_z, Block>::iterator it1;


//--------------------------------------------------------------------------
void SetTests::setUp()
{
}

//--------------------------------------------------------------------------
void SetTests::tearDown()
{
}

//--------------------------------------------------------------------------
void SetTests::testDummy()
{
  // Check dummy container
  set1.debug();
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0_z), std::out_of_range);

  // Check dummy container
  CPPUNIT_ASSERT_EQUAL(0_z, set2.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set2.blocks());
  CPPUNIT_ASSERT_EQUAL(8_z, set2.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set2.empty());
  CPPUNIT_ASSERT_EQUAL(false, set2.full());
  CPPUNIT_ASSERT_EQUAL(true, set2.outofbound(0_z));
  CPPUNIT_ASSERT_THROW(set2.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set2.index());
  CPPUNIT_ASSERT_THROW(set2.get(0_z), std::out_of_range);

  // Useless operations
  set1.remove();
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0_z), std::out_of_range);

  // Useless operations
  set1.remove(1_z);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0_z), std::out_of_range);

  // Useless operations
  CPPUNIT_ASSERT_THROW(set1.modify(0, 42), std::out_of_range);

  // Useless operations
  set1.garbage();
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0_z), std::out_of_range);

  // Useless operations
  set1.clear();
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0_z), std::out_of_range);

  // Try occupy an empty block. Check this is not possible
  CPPUNIT_ASSERT_THROW(set1.occupy(0), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());

  // Try occupy an empty block. Check this is not possible
  set1.append(41); set1.clear();
  CPPUNIT_ASSERT_THROW(set1.occupy(1), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_THROW(set1.occupy(0), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
}

//--------------------------------------------------------------------------
void SetTests::testInsert()
{
  // Insert an element. Check it was inserted.
  set1.append(41);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_THROW(set1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.m_blocks[0]);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.m_blocks[0]->occupation());

  // Modify an element. Check it was changed.
  set1.modify(0, 42);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(42, set1.get(0_z));
  CPPUNIT_ASSERT_THROW(set1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.m_blocks[0]);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.m_blocks[0]->occupation());

  // Modify (restore previous value). Check it was restored.
  set1.modify(0, 41);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_THROW(set1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.m_blocks[0]);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.m_blocks[0]->occupation());

  // Insert an element. Check it was inserted.
  set1.append(42);
  CPPUNIT_ASSERT_EQUAL(2_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(2_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1_z));
  CPPUNIT_ASSERT_THROW(set1.get(2_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(2_z, set1.m_blocks[0]->occupation());

  // Insert an element. Check it was inserted.
  set1.append(43);
  CPPUNIT_ASSERT_EQUAL(3_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(3_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(2_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2_z));
  CPPUNIT_ASSERT_THROW(set1.get(3_z), std::out_of_range);

  // Insert an element. Check it was inserted.
  set1.append(44);
  CPPUNIT_ASSERT_EQUAL(4_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(3_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(3_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2_z));
  CPPUNIT_ASSERT_EQUAL(44, set1.get(3_z));
  CPPUNIT_ASSERT_THROW(set1.get(4_z), std::out_of_range);

  // Insert an element. Check it was inserted.
  set1.append(45);
  CPPUNIT_ASSERT_EQUAL(5_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(4_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(4_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2_z));
  CPPUNIT_ASSERT_EQUAL(44, set1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(45, set1.get(4_z));
  CPPUNIT_ASSERT_THROW(set1.get(5_z), std::out_of_range);

  // Try delete unused memory. Check that all memory is used and cannot be released.
  set1.garbage();
  CPPUNIT_ASSERT_EQUAL(5_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(4_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(4_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2_z));
  CPPUNIT_ASSERT_EQUAL(44, set1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(45, set1.get(4_z));
  CPPUNIT_ASSERT_THROW(set1.get(5_z), std::out_of_range);

  // Check iterators
  const int expected_values[5] =
    {
      41, 42, 43, 44, 45
    };

  int i = 0;
  for (it1 = set1.begin(); it1 != set1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values[i], (*it1));
      ++i;
    }
}

//--------------------------------------------------------------------------
void SetTests::testRemove()
{
  set1.remove(1_z);
  CPPUNIT_ASSERT_EQUAL(4_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(4_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(3_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(3_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(45, set1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2_z));
  CPPUNIT_ASSERT_EQUAL(44, set1.get(3_z));
  CPPUNIT_ASSERT_THROW(set1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5_z), std::out_of_range);

  // Check iterators
  const int expected_values[4] =
    {
      41, 45, 43, 44
    };

  int i = 0;
  for (it1 = set1.begin(); it1 != set1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(expected_values[i], (*it1));
      ++i;
    }

  set1.remove(3_z);
  CPPUNIT_ASSERT_EQUAL(3_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(5_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(2_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(45, set1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2_z));
  CPPUNIT_ASSERT_THROW(set1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5_z), std::out_of_range);

  set1.remove(0_z);
  CPPUNIT_ASSERT_EQUAL(2_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(6_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(43, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(45, set1.get(1_z));
  CPPUNIT_ASSERT_THROW(set1.get(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5_z), std::out_of_range);

  set1.remove(0_z);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(7_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(45, set1.get(0_z));
  CPPUNIT_ASSERT_THROW(set1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5_z), std::out_of_range);

  for (it1 = set1.begin(); it1 != set1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(45, (*it1));
    }

  set1.remove();
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(8_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5_z), std::out_of_range);

  set1.remove(0_z);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(8_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5_z), std::out_of_range);

  set1.garbage();
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(3_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5_z), std::out_of_range);

  for (it1 = set1.begin(); it1 != set1.end(); ++it1)
    {
      CPPUNIT_FAIL("Not supposed to iterate");
    }

  set1.append(41);
  set1.append(42);
  CPPUNIT_ASSERT_EQUAL(2_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(2_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1_z));
  CPPUNIT_ASSERT_THROW(set1.get(2_z), std::out_of_range);

  set1.clear();
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(4_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2_z), std::out_of_range);

  set1.garbage();
  CPPUNIT_ASSERT_EQUAL(0_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2_z), std::out_of_range);
}

//--------------------------------------------------------------------------
void SetTests::testInsertArr()
{
  // Append one element. Check it was inserted.
  set1.append(41);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_THROW(set1.get(1_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.m_blocks[0]);
  CPPUNIT_ASSERT_EQUAL(1_z, set1.m_blocks[0]->occupation());

  // Append an array of elements. Check elements have been inserted.
  const int array[] = { 1, 2, 3 };
  set1.append(array, 3);

  CPPUNIT_ASSERT_EQUAL(4_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(3_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(3_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(4_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(3_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(1, set1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(2, set1.get(2_z));
  CPPUNIT_ASSERT_EQUAL(3, set1.get(3_z));
  CPPUNIT_ASSERT_THROW(set1.get(4_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.block(0));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set1.block(1));
  CPPUNIT_ASSERT_EQUAL(4_z, set1.m_blocks[0]->occupation());

  // Append nullptr array. Check nothing was done.
  set1.append(nullptr, 0);
  CPPUNIT_ASSERT_EQUAL(4_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set1.remaining());

  // Append a vector of elements. Check elements have been inserted.
  std::vector<int> vect = { 4, 5, 6 };
  set1.append(vect);

  CPPUNIT_ASSERT_EQUAL(7_z, set1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(1_z, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(6_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(7_z));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(6_z));
  CPPUNIT_ASSERT_THROW(set1.occupied(7_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(6_z, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0_z));
  CPPUNIT_ASSERT_EQUAL(1, set1.get(1_z));
  CPPUNIT_ASSERT_EQUAL(2, set1.get(2_z));
  CPPUNIT_ASSERT_EQUAL(3, set1.get(3_z));
  CPPUNIT_ASSERT_EQUAL(4, set1.get(4_z));
  CPPUNIT_ASSERT_EQUAL(5, set1.get(5_z));
  CPPUNIT_ASSERT_EQUAL(6, set1.get(6_z));
  CPPUNIT_ASSERT_THROW(set1.get(7_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.block(0));
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.block(1));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set1.block(2));
  CPPUNIT_ASSERT_EQUAL(4_z, set1.m_blocks[0]->occupation());
  CPPUNIT_ASSERT_EQUAL(3_z, set1.m_blocks[1]->occupation());

  // Append more data on different container sizes.
  int array2[64];
  for (int i = 0; i < 64; ++i) array2[i] = i;

  set4.append(array2, 16);
  CPPUNIT_ASSERT_EQUAL(16_z, set4.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set4.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set4.remaining());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set4.block(0));
  CPPUNIT_ASSERT_EQUAL(16_z, set4.m_blocks[0]->occupation());
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set4.block(1));

  // Append more data on different container sizes.
  set5.append(array2, 64);
  CPPUNIT_ASSERT_EQUAL(64_z, set5.used());
  CPPUNIT_ASSERT_EQUAL(2_z, set5.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set5.remaining());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set5.block(0));
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set5.block(1));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set5.block(2));
  CPPUNIT_ASSERT_EQUAL(32_z, set5.m_blocks[0]->occupation());
  CPPUNIT_ASSERT_EQUAL(32_z, set5.m_blocks[1]->occupation());

  //
  set6.append(array2, 64);
  CPPUNIT_ASSERT_EQUAL(64_z, set6.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set6.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set6.remaining());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set6.block(0));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set6.block(1));
  for (int i = 0; i < 64; ++i) { CPPUNIT_ASSERT_EQUAL(i, set6.get(i)); }
  CPPUNIT_ASSERT_EQUAL(64_z, set6.m_blocks[0]->occupation());

  //
  int array3[65536];
  for (int i = 0; i < 65536; ++i) array3[i] = i;
  set7.append(array3, 65536);
  CPPUNIT_ASSERT_EQUAL(65536_z, set7.used());
  CPPUNIT_ASSERT_EQUAL(1_z, set7.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, set7.remaining());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set7.block(0));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set7.block(1));
  for (int i = 0; i < 65536; ++i) { CPPUNIT_ASSERT_EQUAL(i, set7.get(i)); }
  CPPUNIT_ASSERT_EQUAL(65536_z, set7.m_blocks[0]->occupation());
  CPPUNIT_ASSERT_EQUAL(true, set7.m_blocks[0]->E >= 1024); // 1024 for 64-bit archi, 2048 for 32-bits archi ...
  for (int i = 0; i < set7.m_blocks[0]->E; ++i)
  { 
    CPPUNIT_ASSERT_EQUAL(std::numeric_limits<ContainerBitField>::max(), set7.m_blocks[0]->m_occupied[i]);
  }
}
