#include "SetTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(SetTests);

// A container of 2^2 elements by block with 0 block pre-allocated
static Set<int, 2U, Block> set1(0);
// A container of 2^2 elements by block with 5/4 blocks pre-allocated
static Set<int, 2U, Block> set2(5);
// A container of 2^2 elements by block with 0 block pre-allocated
static Set<int, 2U, Block> set3(0);
// A container of 2^4 elements by block with 0 block pre-allocated
static Set<int, 4U, Block> set4(0);
// A container of 2^5 elements by block with 0 block pre-allocated
static Set<int, 5U, Block> set5(0);
// A container of 2^6 elements by block with 0 block pre-allocated
static Set<int, 6U, Block> set6(0);
static Set<int, 2U, Block>::iterator it1;

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
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_EQUAL(0U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0U));
  CPPUNIT_ASSERT_THROW(set1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0U), std::out_of_range);

  // Check dummy container
  CPPUNIT_ASSERT_EQUAL(0U, set2.used());
  CPPUNIT_ASSERT_EQUAL(2U, set2.blocks());
  CPPUNIT_ASSERT_EQUAL(8U, set2.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set2.empty());
  CPPUNIT_ASSERT_EQUAL(false, set2.full());
  CPPUNIT_ASSERT_EQUAL(true, set2.outofbound(0U));
  CPPUNIT_ASSERT_THROW(set2.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set2.index());
  CPPUNIT_ASSERT_THROW(set2.get(0U), std::out_of_range);

  // Useless operations
  set1.remove();
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_EQUAL(0U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0U));
  CPPUNIT_ASSERT_THROW(set1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0U), std::out_of_range);

  // Useless operations
  set1.remove(1U);
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_EQUAL(0U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0U));
  CPPUNIT_ASSERT_THROW(set1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0U), std::out_of_range);

  // Useless operations
  CPPUNIT_ASSERT_THROW(set1.modify(0, 42), std::out_of_range);

  // Useless operations
  set1.garbage();
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_EQUAL(0U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0U));
  CPPUNIT_ASSERT_THROW(set1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0U), std::out_of_range);

  // Useless operations
  set1.clear();
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_EQUAL(0U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0U));
  CPPUNIT_ASSERT_THROW(set1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0U), std::out_of_range);

  // Try occupy an empty block. Check this is not possible
  CPPUNIT_ASSERT_THROW(set1.occupy(0), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());

  // Try occupy an empty block. Check this is not possible
  set1.append(41); set1.clear();
  CPPUNIT_ASSERT_THROW(set1.occupy(1), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_THROW(set1.occupy(0), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
}

//--------------------------------------------------------------------------
void SetTests::testInsert()
{
  // Insert an element. Check it was inserted.
  set1.append(41);
  CPPUNIT_ASSERT_EQUAL(1U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_THROW(set1.occupied(1U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_THROW(set1.get(1U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.m_blocks[0]);
  CPPUNIT_ASSERT_EQUAL(1U, set1.m_blocks[0]->occupation());

  // Modify an element. Check it was changed.
  set1.modify(0, 42);
  CPPUNIT_ASSERT_EQUAL(1U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_THROW(set1.occupied(1U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_EQUAL(42, set1.get(0U));
  CPPUNIT_ASSERT_THROW(set1.get(1U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.m_blocks[0]);
  CPPUNIT_ASSERT_EQUAL(1U, set1.m_blocks[0]->occupation());

  // Modify (restore previous value). Check it was restored.
  set1.modify(0, 41);
  CPPUNIT_ASSERT_EQUAL(1U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_THROW(set1.occupied(1U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_THROW(set1.get(1U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.m_blocks[0]);
  CPPUNIT_ASSERT_EQUAL(1U, set1.m_blocks[0]->occupation());

  // Insert an element. Check it was inserted.
  set1.append(42);
  CPPUNIT_ASSERT_EQUAL(2U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(2U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1U));
  CPPUNIT_ASSERT_THROW(set1.occupied(2U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(1U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1U));
  CPPUNIT_ASSERT_THROW(set1.get(2U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(2U, set1.m_blocks[0]->occupation());

  // Insert an element. Check it was inserted.
  set1.append(43);
  CPPUNIT_ASSERT_EQUAL(3U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(1U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2U));
  CPPUNIT_ASSERT_THROW(set1.occupied(3U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(2U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1U));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2U));
  CPPUNIT_ASSERT_THROW(set1.get(3U), std::out_of_range);

  // Insert an element. Check it was inserted.
  set1.append(44);
  CPPUNIT_ASSERT_EQUAL(4U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(3U));
  CPPUNIT_ASSERT_THROW(set1.occupied(4U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(3U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1U));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2U));
  CPPUNIT_ASSERT_EQUAL(44, set1.get(3U));
  CPPUNIT_ASSERT_THROW(set1.get(4U), std::out_of_range);

  // Insert an element. Check it was inserted.
  set1.append(45);
  CPPUNIT_ASSERT_EQUAL(5U, set1.used());
  CPPUNIT_ASSERT_EQUAL(2U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(4U));
  CPPUNIT_ASSERT_THROW(set1.occupied(5U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(4U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1U));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2U));
  CPPUNIT_ASSERT_EQUAL(44, set1.get(3U));
  CPPUNIT_ASSERT_EQUAL(45, set1.get(4U));
  CPPUNIT_ASSERT_THROW(set1.get(5U), std::out_of_range);

  // Try delete unused memory. Check that all memory is used and cannot be released.
  set1.garbage();
  CPPUNIT_ASSERT_EQUAL(5U, set1.used());
  CPPUNIT_ASSERT_EQUAL(2U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(4U));
  CPPUNIT_ASSERT_THROW(set1.occupied(5U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(4U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1U));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2U));
  CPPUNIT_ASSERT_EQUAL(44, set1.get(3U));
  CPPUNIT_ASSERT_EQUAL(45, set1.get(4U));
  CPPUNIT_ASSERT_THROW(set1.get(5U), std::out_of_range);

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
  set1.remove(1U);
  CPPUNIT_ASSERT_EQUAL(4U, set1.used());
  CPPUNIT_ASSERT_EQUAL(2U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(4U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(3U));
  CPPUNIT_ASSERT_THROW(set1.occupied(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(3U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(45, set1.get(1U));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2U));
  CPPUNIT_ASSERT_EQUAL(44, set1.get(3U));
  CPPUNIT_ASSERT_THROW(set1.get(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5U), std::out_of_range);

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

  set1.remove(3U);
  CPPUNIT_ASSERT_EQUAL(3U, set1.used());
  CPPUNIT_ASSERT_EQUAL(2U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(5U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(2U));
  CPPUNIT_ASSERT_THROW(set1.occupied(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(2U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(45, set1.get(1U));
  CPPUNIT_ASSERT_EQUAL(43, set1.get(2U));
  CPPUNIT_ASSERT_THROW(set1.get(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5U), std::out_of_range);

  set1.remove(0U);
  CPPUNIT_ASSERT_EQUAL(2U, set1.used());
  CPPUNIT_ASSERT_EQUAL(2U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(6U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1U));
  CPPUNIT_ASSERT_THROW(set1.occupied(2U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(1U, set1.index());
  CPPUNIT_ASSERT_EQUAL(43, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(45, set1.get(1U));
  CPPUNIT_ASSERT_THROW(set1.get(2U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5U), std::out_of_range);

  set1.remove(0U);
  CPPUNIT_ASSERT_EQUAL(1U, set1.used());
  CPPUNIT_ASSERT_EQUAL(2U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(7U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_THROW(set1.occupied(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_EQUAL(45, set1.get(0U));
  CPPUNIT_ASSERT_THROW(set1.get(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5U), std::out_of_range);

  for (it1 = set1.begin(); it1 != set1.end(); ++it1)
    {
      CPPUNIT_ASSERT_EQUAL(45, (*it1));
    }

  set1.remove();
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_EQUAL(2U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(8U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5U));
  CPPUNIT_ASSERT_THROW(set1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5U), std::out_of_range);

  set1.remove(0U);
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_EQUAL(2U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(8U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5U));
  CPPUNIT_ASSERT_THROW(set1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5U), std::out_of_range);

  set1.garbage();
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_EQUAL(0U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(5U));
  CPPUNIT_ASSERT_THROW(set1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(5U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(3U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(4U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(5U), std::out_of_range);

  for (it1 = set1.begin(); it1 != set1.end(); ++it1)
    {
      CPPUNIT_FAIL("Not supposed to iterate");
    }

  set1.append(41);
  set1.append(42);
  CPPUNIT_ASSERT_EQUAL(2U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(2U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(1U));
  CPPUNIT_ASSERT_THROW(set1.occupied(2U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(1U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(42, set1.get(1U));
  CPPUNIT_ASSERT_THROW(set1.get(2U), std::out_of_range);

  set1.clear();
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(4U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2U));
  CPPUNIT_ASSERT_THROW(set1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2U), std::out_of_range);

  set1.garbage();
  CPPUNIT_ASSERT_EQUAL(0U, set1.used());
  CPPUNIT_ASSERT_EQUAL(0U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(2U));
  CPPUNIT_ASSERT_THROW(set1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.occupied(2U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_THROW(set1.get(0U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(1U), std::out_of_range);
  CPPUNIT_ASSERT_THROW(set1.get(2U), std::out_of_range);
}

//--------------------------------------------------------------------------
void SetTests::testInsertArr()
{
  // Append one element. Check it was inserted.
  set1.append(41);
  CPPUNIT_ASSERT_EQUAL(1U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(3U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(0U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(1U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(0U));
  CPPUNIT_ASSERT_THROW(set1.occupied(1U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_THROW(set1.get(1U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.m_blocks[0]);
  CPPUNIT_ASSERT_EQUAL(1U, set1.m_blocks[0]->occupation());

  // Append an array of elements. Check elements have been inserted.
  const int array[] = { 1, 2, 3 };
  set1.append(array, 3);

  CPPUNIT_ASSERT_EQUAL(4U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(true, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(3U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(4U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(3U));
  CPPUNIT_ASSERT_THROW(set1.occupied(4U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(3U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(1, set1.get(1U));
  CPPUNIT_ASSERT_EQUAL(2, set1.get(2U));
  CPPUNIT_ASSERT_EQUAL(3, set1.get(3U));
  CPPUNIT_ASSERT_THROW(set1.get(4U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.block(0));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set1.block(1));
  CPPUNIT_ASSERT_EQUAL(4U, set1.m_blocks[0]->occupation());

  // Append nullptr array. Check nothing was done.
  set1.append(nullptr, 0);
  CPPUNIT_ASSERT_EQUAL(4U, set1.used());
  CPPUNIT_ASSERT_EQUAL(1U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set1.remaining());

  // Append a vector of elements. Check elements have been inserted.
  std::vector<int> vect = { 4, 5, 6 };
  set1.append(vect);

  CPPUNIT_ASSERT_EQUAL(7U, set1.used());
  CPPUNIT_ASSERT_EQUAL(2U, set1.blocks());
  CPPUNIT_ASSERT_EQUAL(1U, set1.remaining());
  CPPUNIT_ASSERT_EQUAL(false, set1.empty());
  CPPUNIT_ASSERT_EQUAL(false, set1.full());
  CPPUNIT_ASSERT_EQUAL(false, set1.outofbound(6U));
  CPPUNIT_ASSERT_EQUAL(true, set1.outofbound(7U));
  CPPUNIT_ASSERT_EQUAL(true, set1.occupied(6U));
  CPPUNIT_ASSERT_THROW(set1.occupied(7U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(6U, set1.index());
  CPPUNIT_ASSERT_EQUAL(41, set1.get(0U));
  CPPUNIT_ASSERT_EQUAL(1, set1.get(1U));
  CPPUNIT_ASSERT_EQUAL(2, set1.get(2U));
  CPPUNIT_ASSERT_EQUAL(3, set1.get(3U));
  CPPUNIT_ASSERT_EQUAL(4, set1.get(4U));
  CPPUNIT_ASSERT_EQUAL(5, set1.get(5U));
  CPPUNIT_ASSERT_EQUAL(6, set1.get(6U));
  CPPUNIT_ASSERT_THROW(set1.get(7U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.block(0));
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set1.block(1));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set1.block(2));
  CPPUNIT_ASSERT_EQUAL(4U, set1.m_blocks[0]->occupation());
  CPPUNIT_ASSERT_EQUAL(3U, set1.m_blocks[1]->occupation());

  // Append more data on different container sizes.
  const int array2[64] = { 0 };
  set4.append(array2, 16);
  CPPUNIT_ASSERT_EQUAL(16U, set4.used());
  CPPUNIT_ASSERT_EQUAL(1U, set4.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set4.remaining());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set4.block(0));
  CPPUNIT_ASSERT_EQUAL(16U, set4.m_blocks[0]->occupation());
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set4.block(1));

  // Append more data on different container sizes.
  set5.append(array2, 64);
  CPPUNIT_ASSERT_EQUAL(64U, set5.used());
  CPPUNIT_ASSERT_EQUAL(2U, set5.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set5.remaining());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set5.block(0));
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set5.block(1));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set5.block(2));
  CPPUNIT_ASSERT_EQUAL(32U, set5.m_blocks[0]->occupation());
  CPPUNIT_ASSERT_EQUAL(32U, set5.m_blocks[0]->occupation());

  set6.append(array2, 64);
  CPPUNIT_ASSERT_EQUAL(64U, set6.used());
  CPPUNIT_ASSERT_EQUAL(1U, set6.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, set6.remaining());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != set6.block(0));
  CPPUNIT_ASSERT_EQUAL(64U, set6.m_blocks[0]->occupation());
  CPPUNIT_ASSERT_EQUAL(true, nullptr == set6.block(1));
}
