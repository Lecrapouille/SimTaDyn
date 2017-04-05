#include "GLBufferTests.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(GLBufferTests);

// A generic OpenGL data block of 2^2 GPU elements specialized for VertexBuffer
static GLBlockBuffer<int, 2U> block1(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
// A vertex Buffer block of data
static GLVertexBlockBuffer<int, 2U> block2(GL_DYNAMIC_DRAW);
// An index Buffer block of data
static GLIndexBlockBuffer<int, 2U> block3(GL_STATIC_DRAW);
//
static GLIndexBuffer<int, 2U> buf1(0U);
//
static GLVertexBuffer<int, 2U> buf2;


//--------------------------------------------------------------------------
void GLBufferTests::setUp()
{
  // FIXME init OpenGL context to avoid segfault during destructor
}

//--------------------------------------------------------------------------
void GLBufferTests::tearDown()
{
}

//--------------------------------------------------------------------------
void GLBufferTests::testDummy()
{
  // GLObject
  CPPUNIT_ASSERT_EQUAL(true, GL_ARRAY_BUFFER == block1.m_target);
  CPPUNIT_ASSERT_EQUAL(true, GL_DYNAMIC_DRAW == block1.m_usage);
  CPPUNIT_ASSERT_EQUAL(0U, block1.cpuID());
  CPPUNIT_ASSERT_EQUAL(true, 0U == block1.gpuID());
  CPPUNIT_ASSERT_EQUAL(true, block1.m_need_setup);
  CPPUNIT_ASSERT_EQUAL(true, block1.m_need_create);
  CPPUNIT_ASSERT_EQUAL(true, block1.m_need_update);
  CPPUNIT_ASSERT_EQUAL(false, block1.m_need_delete);

  // GLObject
  CPPUNIT_ASSERT_EQUAL(true, GL_ARRAY_BUFFER == block2.m_target);
  CPPUNIT_ASSERT_EQUAL(true, GL_DYNAMIC_DRAW == block2.m_usage);
  CPPUNIT_ASSERT_EQUAL(1U, block2.cpuID());
  CPPUNIT_ASSERT_EQUAL(true, 0U == block2.gpuID());
  CPPUNIT_ASSERT_EQUAL(true, block2.m_need_setup);
  CPPUNIT_ASSERT_EQUAL(true, block2.m_need_create);
  CPPUNIT_ASSERT_EQUAL(true, block2.m_need_update);
  CPPUNIT_ASSERT_EQUAL(false, block2.m_need_delete);

  // GLObject
  CPPUNIT_ASSERT_EQUAL(true, GL_ELEMENT_ARRAY_BUFFER == block3.m_target);
  CPPUNIT_ASSERT_EQUAL(true, GL_STATIC_DRAW == block3.m_usage);
  CPPUNIT_ASSERT_EQUAL(2U, block3.cpuID());
  CPPUNIT_ASSERT_EQUAL(true, 0U == block3.gpuID());
  CPPUNIT_ASSERT_EQUAL(true, block3.m_need_setup);
  CPPUNIT_ASSERT_EQUAL(true, block3.m_need_create);
  CPPUNIT_ASSERT_EQUAL(true, block3.m_need_update);
  CPPUNIT_ASSERT_EQUAL(false, block3.m_need_delete);

  // Empty block
  CPPUNIT_ASSERT_EQUAL(4U, block1.elements());
  uint32_t i = Block<int, 2U>::E;
  while (i--)
    {
      CPPUNIT_ASSERT_EQUAL(0U, block1.m_occupied[i]);
    }

  // No pending data
  CPPUNIT_ASSERT_EQUAL((uint32_t) -1, block1.m_pending_start);
  CPPUNIT_ASSERT_EQUAL((uint32_t) -1, block1.m_pending_end);
  uint32_t pos_start = 40U;
  uint32_t pos_end = 41U;
  CPPUNIT_ASSERT_EQUAL(false, block1.hasPendingData(pos_start, pos_end));
  CPPUNIT_ASSERT_EQUAL(40U, pos_start);
  CPPUNIT_ASSERT_EQUAL(41U, pos_end);
  CPPUNIT_ASSERT_EQUAL(false, block1.needUpdate());

  // Dummy Set
  CPPUNIT_ASSERT_EQUAL(0U, buf1.used());
  CPPUNIT_ASSERT_EQUAL(0U, buf1.blocks());
  CPPUNIT_ASSERT_EQUAL(0U, buf1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, buf1.empty());
  CPPUNIT_ASSERT_EQUAL(true, buf1.full());
  CPPUNIT_ASSERT_EQUAL(true, buf1.outofbound(0U));
  CPPUNIT_ASSERT_THROW(buf1.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, buf1.index());
  CPPUNIT_ASSERT_THROW(buf1.get(0U), std::out_of_range);

  // Dummy Set
  CPPUNIT_ASSERT_EQUAL(0U, buf2.used());
  CPPUNIT_ASSERT_EQUAL(1U, buf2.blocks());
  CPPUNIT_ASSERT_EQUAL(4U, buf2.remaining());
  CPPUNIT_ASSERT_EQUAL(true, buf2.empty());
  CPPUNIT_ASSERT_EQUAL(false, buf2.full());
  CPPUNIT_ASSERT_EQUAL(true, buf2.outofbound(0U));
  CPPUNIT_ASSERT_THROW(buf2.occupied(0U), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0U, buf2.index());
  CPPUNIT_ASSERT_THROW(buf2.get(0U), std::out_of_range);

  buf2.m_blocks[0]->addPendingData(pos_start, pos_end);
  pos_start = 0U;
  pos_end = 1U;
  CPPUNIT_ASSERT_EQUAL(true, buf2.m_blocks[0]->hasPendingData(pos_start, pos_end));
  CPPUNIT_ASSERT_EQUAL(40U, pos_start);
  CPPUNIT_ASSERT_EQUAL(41U, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, buf2.m_blocks[0]->needUpdate());
}

//--------------------------------------------------------------------------
void GLBufferTests::testPendingData()
{
  uint32_t pos_start = 40U;
  uint32_t pos_end = 41U;

  // Pending data
  block1.addPendingData(pos_start, pos_end);
  pos_start = 0U;
  pos_end = 1U;
  CPPUNIT_ASSERT_EQUAL(true, block1.hasPendingData(pos_start, pos_end));
  CPPUNIT_ASSERT_EQUAL(40U, pos_start);
  CPPUNIT_ASSERT_EQUAL(41U, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, block1.needUpdate());

  // No pending data
  block1.clearPending();
  CPPUNIT_ASSERT_EQUAL((uint32_t) -1, block1.m_pending_start);
  CPPUNIT_ASSERT_EQUAL((uint32_t) -1, block1.m_pending_end);
  CPPUNIT_ASSERT_EQUAL(false, block1.hasPendingData(pos_start, pos_end));
  CPPUNIT_ASSERT_EQUAL(40U, pos_start);
  CPPUNIT_ASSERT_EQUAL(41U, pos_end);
  CPPUNIT_ASSERT_EQUAL(false, block1.needUpdate());

  // Pending data
  pos_start = 40U;
  pos_end = 41U;
  block1.addPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, block1.hasPendingData(pos_start, pos_end));
  CPPUNIT_ASSERT_EQUAL(40U, pos_start);
  CPPUNIT_ASSERT_EQUAL(41U, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, block1.needUpdate());

  // Extending pending data
  pos_start = 40U;
  pos_end = 42U;
  block1.addPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, block1.hasPendingData(pos_start, pos_end));
  CPPUNIT_ASSERT_EQUAL(40U, pos_start);
  CPPUNIT_ASSERT_EQUAL(42U, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, block1.needUpdate());

  // Extending pending data
  pos_start = 2U;
  pos_end = 44U;
  block1.addPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, block1.hasPendingData(pos_start, pos_end));
  CPPUNIT_ASSERT_EQUAL(2U, pos_start);
  CPPUNIT_ASSERT_EQUAL(44U, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, block1.needUpdate());
}

//--------------------------------------------------------------------------
void GLBufferTests::testInsert()
{
  uint32_t pos_start = 40U;
  uint32_t pos_end = 41U;

  buf1.append(41);
  auto b = buf1.m_blocks[0];

  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData(pos_start, pos_end));
  CPPUNIT_ASSERT_EQUAL(0U, pos_start);
  CPPUNIT_ASSERT_EQUAL(0U, pos_end);
  CPPUNIT_ASSERT_EQUAL(0U, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(0U, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  buf1.append(42);
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData(pos_start, pos_end));
  CPPUNIT_ASSERT_EQUAL(0U, pos_start);
  CPPUNIT_ASSERT_EQUAL(1U, pos_end);
  CPPUNIT_ASSERT_EQUAL(0U, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(1U, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());
}
