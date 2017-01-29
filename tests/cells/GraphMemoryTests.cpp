#include "GraphMemoryTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(GraphMemoryTests);

//--------------------------------------------------------------------------
void GraphMemoryTests::setUp()
{
}

//--------------------------------------------------------------------------
void GraphMemoryTests::tearDown()
{
}

//--------------------------------------------------------------------------
void GraphMemoryTests::test()
{
  typedef uint32_t T;

  // Empty container with one memory pool.
  container<T> c1;
  const container<T> *ptr = &c1;
  CPPUNIT_ASSERT_EQUAL(1U, c1.allocated());
  CPPUNIT_ASSERT_EQUAL(0U, c1.occupation());
  CPPUNIT_ASSERT_EQUAL(true, c1.empty());
  CPPUNIT_ASSERT_EQUAL(false, c1.full());

  // Check all element are not occupied
  uint32_t i = c1.poolSizeAllocation();
  while (i--)
    {
      CPPUNIT_ASSERT_EQUAL(false, c1.isOccupied(i));
      CPPUNIT_ASSERT_EQUAL(true, c1.isValidIndex(i));

      // Trying to access to element reach an error
      try
        {
          T n = c1.get(i);
          std::cout << n << std::endl;
          CPPUNIT_FAIL("Exception did not occured");
        }
      catch (std::out_of_range const&)
        {
          CPPUNIT_ASSERT_EQUAL(0U, c1.m_index);
        }
    }

  // Check that trying to remove the last element is usless
  CPPUNIT_ASSERT_EQUAL(0U, c1.m_index);
  c1.remove();
  CPPUNIT_ASSERT_EQUAL(0U, c1.m_index);

  // Check begin and end iterators are the same
  container<T>::iterator begin = c1.begin();
  container<T>::iterator end = c1.end();

  CPPUNIT_ASSERT_EQUAL(ptr, begin.m_container);
  CPPUNIT_ASSERT_EQUAL(0U, begin.m_itr);
  CPPUNIT_ASSERT_EQUAL(ptr, end.m_container);
  CPPUNIT_ASSERT_EQUAL(0U, end.m_itr);
  CPPUNIT_ASSERT_EQUAL(true, (c1.end() == c1.begin()));

  // Check trying to go to next element is useless
  ++begin;
  CPPUNIT_ASSERT_EQUAL(ptr, begin.m_container);
  CPPUNIT_ASSERT_EQUAL(0U, begin.m_itr);
  CPPUNIT_ASSERT_EQUAL(true, (c1.end() == begin));
  CPPUNIT_ASSERT_EQUAL(true, c1.empty());
  CPPUNIT_ASSERT_EQUAL(false, c1.full());

  // Check trying to go to next element is useless
  --begin;
  CPPUNIT_ASSERT_EQUAL(ptr, begin.m_container);
  CPPUNIT_ASSERT_EQUAL(0U, begin.m_itr);
  CPPUNIT_ASSERT_EQUAL(true, (c1.end() == begin));
  CPPUNIT_ASSERT_EQUAL(true, c1.empty());
  CPPUNIT_ASSERT_EQUAL(false, c1.full());

  for (i = 0; i < c1.poolSizeAllocation(); ++i)
    {
      c1.append(i);
      CPPUNIT_ASSERT_EQUAL(i, c1.get(i));
      CPPUNIT_ASSERT_EQUAL(1U, c1.allocated());
      CPPUNIT_ASSERT_EQUAL(i + 1U, c1.occupation());
      CPPUNIT_ASSERT_EQUAL(false, c1.empty());
      CPPUNIT_ASSERT_EQUAL(((i + 1U) >= c1.poolSizeAllocation()), c1.full());
    }
  CPPUNIT_ASSERT_EQUAL(true, c1.full());
  c1.append(i);
  CPPUNIT_ASSERT_EQUAL(2U, c1.allocated());
  CPPUNIT_ASSERT_EQUAL(i + 1U, c1.occupation());
  CPPUNIT_ASSERT_EQUAL(i, c1.get(i));

  // Remove element
  c1.remove();
  CPPUNIT_ASSERT_EQUAL(2U, c1.allocated());
  CPPUNIT_ASSERT_EQUAL(i, c1.occupation());
  try
    {
      T n = c1.get(i);
      std::cout << n << std::endl;
      CPPUNIT_FAIL("Exception did not occured");
    }
  catch (std::out_of_range const&)
    {
    }

  // Check iterators
  begin = c1.begin();
  CPPUNIT_ASSERT_EQUAL(ptr, begin.m_container);
  CPPUNIT_ASSERT_EQUAL(0U, begin.m_itr);
  end = c1.end();
  CPPUNIT_ASSERT_EQUAL(ptr, end.m_container);
  CPPUNIT_ASSERT_EQUAL(c1.poolSizeAllocation(), end.m_itr);
  container<T>::iterator back = c1.back();
  CPPUNIT_ASSERT_EQUAL(ptr, back.m_container);
  CPPUNIT_ASSERT_EQUAL(c1.poolSizeAllocation() - 1U, back.m_itr);
  CPPUNIT_ASSERT_EQUAL(c1.poolSizeAllocation() - 1U, (*back));
  CPPUNIT_ASSERT_EQUAL(true, end != back);
  --end;
  CPPUNIT_ASSERT_EQUAL(true, end == back);

  // Clear
  c1.clear();
  CPPUNIT_ASSERT_EQUAL(true, c1.empty());
  CPPUNIT_ASSERT_EQUAL(false, c1.full());
  CPPUNIT_ASSERT_EQUAL(2U, c1.allocated());
  CPPUNIT_ASSERT_EQUAL(0U, c1.occupation());

  // Insert
  c1.insert(4 * c1.poolSizeAllocation(), 42U);
  CPPUNIT_ASSERT_EQUAL(false, c1.empty());
  CPPUNIT_ASSERT_EQUAL(false, c1.full());
  CPPUNIT_ASSERT_EQUAL(5U, c1.allocated());
  CPPUNIT_ASSERT_EQUAL(1U, c1.occupation());
  CPPUNIT_ASSERT_EQUAL(42U, c1.get(4 * c1.poolSizeAllocation()));

  // Accessor
  c1.insert(2, 66U);
  CPPUNIT_ASSERT_EQUAL(66U, c1.get(2));
  c1.insert(4, 67U);
  CPPUNIT_ASSERT_EQUAL(67U, c1.get(4));

  // Iterator
  T var[16];
  i = 0;
  end = c1.end();
  for (auto it = c1.begin(); it != end; ++it)
    {
      var[i] = (*it);
      std::cout << (*it) << std::endl;
      ++i;
      // No buffer overflow
      CPPUNIT_ASSERT_EQUAL(true, (i < 16));
    }
  CPPUNIT_ASSERT_EQUAL(3U, i);
  CPPUNIT_ASSERT_EQUAL(66U, var[0]);
  CPPUNIT_ASSERT_EQUAL(67U, var[1]);
  CPPUNIT_ASSERT_EQUAL(42U, var[2]);

  //
  var[0] = var[1] = var[2] = 0;
  c1.remove(4);
  CPPUNIT_ASSERT_EQUAL(0U, c1.m_index);
  c1.remove();
  CPPUNIT_ASSERT_EQUAL(0U, c1.m_index);

  i = 0;
  end = c1.end();
  for (auto it = c1.begin(); it != end; ++it)
    {
      var[i] = (*it);
      ++i;
      // No buffer overflow
      CPPUNIT_ASSERT_EQUAL(true, (i < 16));
    }
  c1.debug();
  CPPUNIT_ASSERT_EQUAL(2U, i);
  CPPUNIT_ASSERT_EQUAL(66U, var[0]);
  CPPUNIT_ASSERT_EQUAL(42U, var[1]);
  CPPUNIT_ASSERT_EQUAL(0U, var[2]);

  c1.debug();
}
