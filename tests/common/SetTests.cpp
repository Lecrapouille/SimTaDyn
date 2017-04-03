#include "SetTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(SetTests);

// A container of 2^2 elements by block with 0 block pre-allocated
static Set<int, 2U, Block> set1(0);
// A container of 2^2 elements by block with 5/4 blocks pre-allocated
static Set<int, 2U, Block> set2(5);
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
}

//--------------------------------------------------------------------------
void SetTests::testInsert()
{
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
