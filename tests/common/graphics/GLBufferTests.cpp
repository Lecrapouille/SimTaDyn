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

#include "GLBufferTests.hpp"
#include "GLWindow.hpp"

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(GLBufferTests);

#define INITIAL_INDEX  static_cast<size_t>(-1)

// This class is made for creating an OpenGL context
class GLWindow: public IGLWindow
{
public:

  GLWindow()
    : block1(true, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      block2(true, GL_DYNAMIC_DRAW),
      block3(true, GL_STATIC_DRAW),
      vbo1(0_z)
  {
    std::cout << "GLWindow()" << std::endl;
  }

  ~GLWindow()
  {
    std::cout << "~GLWindow()" << std::endl;
  }

  virtual bool setup() override { return true; }
  virtual bool draw() override { return true; }

  // A generic OpenGL data block of 2^2 GPU elements specialized for VertexBuffer
  GLBlockBuffer<int, 2_z> block1;
  // A vertex Buffer block of data
  GLVertexBlockBuffer<int, 2_z> block2;
  // An index Buffer block of data
  GLIndexBlockBuffer<int, 2_z> block3;
  GLIndexCollection<int, 2_z> vbo1;
  GLVertexCollection<int, 2_z> vbo2;
};

static std::shared_ptr<GLWindow> win;

//--------------------------------------------------------------------------
void GLBufferTests::createOpenGLContext()
{
  win = std::make_shared<GLWindow>();
}

//--------------------------------------------------------------------------
void GLBufferTests::deleteOpenGLContext()
{
  win.reset();
}

//--------------------------------------------------------------------------
void GLBufferTests::setUp()
{
}

//--------------------------------------------------------------------------
void GLBufferTests::tearDown()
{
}

//--------------------------------------------------------------------------
void GLBufferTests::testDummy()
{
  // GLObject
  CPPUNIT_ASSERT_EQUAL(true, GL_ARRAY_BUFFER == win->block1.m_target);
  CPPUNIT_ASSERT_EQUAL(true, GL_DYNAMIC_DRAW == win->block1.m_usage);
  CPPUNIT_ASSERT_EQUAL(true, 0_z == win->block1.getID());
  CPPUNIT_ASSERT_EQUAL(true, win->block1.needSetup());
  CPPUNIT_ASSERT_EQUAL(true, win->block1.needCreate());
  CPPUNIT_ASSERT_EQUAL(false, win->block1.needUpdate());
  CPPUNIT_ASSERT_EQUAL(false, win->block1.canBeReleased());

  // GLObject
  CPPUNIT_ASSERT_EQUAL(true, GL_ARRAY_BUFFER == win->block2.m_target);
  CPPUNIT_ASSERT_EQUAL(true, GL_DYNAMIC_DRAW == win->block2.m_usage);
  CPPUNIT_ASSERT_EQUAL(true, 0_z == win->block2.getID());
  CPPUNIT_ASSERT_EQUAL(true, win->block2.needSetup());
  CPPUNIT_ASSERT_EQUAL(true, win->block2.needCreate());
  CPPUNIT_ASSERT_EQUAL(false, win->block2.needUpdate());
  CPPUNIT_ASSERT_EQUAL(false, win->block2.canBeReleased());

  // GLObject
  CPPUNIT_ASSERT_EQUAL(true, GL_ELEMENT_ARRAY_BUFFER == win->block3.m_target);
  CPPUNIT_ASSERT_EQUAL(true, GL_STATIC_DRAW == win->block3.m_usage);
  CPPUNIT_ASSERT_EQUAL(true, 0_z == win->block3.getID());
  CPPUNIT_ASSERT_EQUAL(true, win->block3.m_need_setup);
  CPPUNIT_ASSERT_EQUAL(true, win->block3.m_need_create);
  CPPUNIT_ASSERT_EQUAL(true, win->block3.m_need_update);
  CPPUNIT_ASSERT_EQUAL(false, win->block3.canBeReleased());

  // Empty win->block
  CPPUNIT_ASSERT_EQUAL(4_z, win->block1.size());
  size_t i = Block<int, 2_z>::E;
  while (i--)
    {
      CPPUNIT_ASSERT_EQUAL(0_z, win->block1.m_occupied[i]);
    }
  CPPUNIT_ASSERT_EQUAL(0_z, win->block1.occupation());

  // No pending data
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, win->block1.m_pending_start);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, win->block1.m_pending_end);
  size_t pos_start = 40_z;
  size_t pos_end = 41_z;
  CPPUNIT_ASSERT_EQUAL(false, win->block1.hasPendingData());
  win->block1.getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(false, win->block1.needUpdate());

  // Dummy Set
  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, win->vbo1.empty());
  CPPUNIT_ASSERT_EQUAL(true, win->vbo1.full());
  CPPUNIT_ASSERT_EQUAL(true, win->vbo1.outofbound(0_z));
  CPPUNIT_ASSERT_THROW(win->vbo1.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo1.index());
  CPPUNIT_ASSERT_THROW(win->vbo1.get(0_z), std::out_of_range);

  // Dummy Set
  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo2.used());
  CPPUNIT_ASSERT_EQUAL(1_z, win->vbo2.blocks());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != win->vbo2.block(0));
  CPPUNIT_ASSERT_EQUAL(4_z, win->vbo2.remaining());
  CPPUNIT_ASSERT_EQUAL(true, win->vbo2.empty());
  CPPUNIT_ASSERT_EQUAL(false, win->vbo2.full());
  CPPUNIT_ASSERT_EQUAL(true, win->vbo2.outofbound(0_z));
  CPPUNIT_ASSERT_THROW(win->vbo2.occupied(0_z), std::out_of_range);
  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo2.index());
  CPPUNIT_ASSERT_THROW(win->vbo2.get(0_z), std::out_of_range);

  // Pending data
  auto b = win->vbo2.block(0);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(false, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_end);

  b->tagAsPending(pos_start, pos_end);
  pos_start = 0_z;
  pos_end = 1_z;
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());
  b->clearPending();
  CPPUNIT_ASSERT_EQUAL(false, b->hasPendingData());
  CPPUNIT_ASSERT_EQUAL(false, b->needUpdate());
}

//--------------------------------------------------------------------------
void GLBufferTests::testPendingData()
{
  size_t pos_start = 40_z;
  size_t pos_end = 41_z;

  // Pending data
  win->block1.tagAsPending(pos_start, pos_end);
  pos_start = 0_z;
  pos_end = 1_z;
  CPPUNIT_ASSERT_EQUAL(true, win->block1.hasPendingData());
  win->block1.getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(40_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(41_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, win->block1.needUpdate());

  // No pending data
  win->block1.clearPending();
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, win->block1.m_pending_start);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, win->block1.m_pending_end);
  CPPUNIT_ASSERT_EQUAL(false, win->block1.hasPendingData());
  win->block1.getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(false, win->block1.needUpdate());

  // Pending data
  pos_start = 40_z;
  pos_end = 41_z;
  win->block1.tagAsPending(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, win->block1.hasPendingData());
  win->block1.getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(40_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(41_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, win->block1.needUpdate());

  // Extending pending data
  pos_start = 40_z;
  pos_end = 42_z;
  win->block1.tagAsPending(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, win->block1.hasPendingData());
  win->block1.getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(40_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(42_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, win->block1.needUpdate());

  // Extending pending data
  pos_start = 2_z;
  pos_end = 44_z;
  win->block1.tagAsPending(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, win->block1.hasPendingData());
  win->block1.getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(2_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(44_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(true, win->block1.needUpdate());
}

//--------------------------------------------------------------------------
void GLBufferTests::testInsert()
{
  size_t pos_start = 40_z;
  size_t pos_end = 41_z;

  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo1.used());
  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo1.remaining());

  win->vbo1.append(41);
  auto b = win->vbo1.block(0);
  CPPUNIT_ASSERT_EQUAL(1_z, win->vbo1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, win->vbo1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, win->vbo1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  win->vbo1.append(42);
  CPPUNIT_ASSERT_EQUAL(2_z, win->vbo1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, win->vbo1.blocks());
  CPPUNIT_ASSERT_EQUAL(2_z, win->vbo1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(1_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(1_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  win->vbo1.append(43);
  CPPUNIT_ASSERT_EQUAL(3_z, win->vbo1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, win->vbo1.blocks());
  CPPUNIT_ASSERT_EQUAL(1_z, win->vbo1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(2_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(2_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  win->vbo1.append(44);
  CPPUNIT_ASSERT_EQUAL(4_z, win->vbo1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, win->vbo1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(3_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(3_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  win->vbo1.remove();
  CPPUNIT_ASSERT_EQUAL(3_z, win->vbo1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, win->vbo1.blocks());
  CPPUNIT_ASSERT_EQUAL(1_z, win->vbo1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(3_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(3_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  b->clearPending();
  CPPUNIT_ASSERT_EQUAL(false, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(false, b->needUpdate());

  win->vbo1.append(45);
  CPPUNIT_ASSERT_EQUAL(4_z, win->vbo1.used());
  CPPUNIT_ASSERT_EQUAL(1_z, win->vbo1.blocks());
  CPPUNIT_ASSERT_EQUAL(0_z, win->vbo1.remaining());
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(3_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(3_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(3_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(3_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  win->vbo1.append(46);
  CPPUNIT_ASSERT_EQUAL(5_z, win->vbo1.used());
  CPPUNIT_ASSERT_EQUAL(2_z, win->vbo1.blocks());
  CPPUNIT_ASSERT_EQUAL(3_z, win->vbo1.remaining());
  b = win->vbo1.block(0);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != b);
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(3_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(3_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(3_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(3_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());
  b = win->vbo1.block(1);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != b);
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  win->vbo1.append(47);
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(1_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(1_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  win->vbo1.append(48);
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(2_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(2_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());
}

//--------------------------------------------------------------------------
void GLBufferTests::testSuppress()
{
  size_t pos_start = 40_z;
  size_t pos_end = 41_z;

  // Pending data activated. Remove the last element. Check pending data indexes did not change.
  win->vbo1.remove();
  auto b = win->vbo1.block(0);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != b);
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(3_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(3_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(3_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(3_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  b = win->vbo1.block(1);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != b);
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, pos_start);
  CPPUNIT_ASSERT_EQUAL(2_z, pos_end);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(2_z, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());

  // Pending data not activated. Remove the last element. Check pending data indexes did not change.
  b->clearPending();
  CPPUNIT_ASSERT_EQUAL(false, b->needUpdate());
  CPPUNIT_ASSERT_EQUAL(false, b->hasPendingData());
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_end);

  win->vbo1.remove();
  CPPUNIT_ASSERT_EQUAL(false, b->hasPendingData());
  b->getPendingData(pos_start, pos_end);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_end);
  CPPUNIT_ASSERT_EQUAL(false, b->needUpdate());

  // Pending data not activated. Remove an element but the last. Check pending data indexes changed.
  b = win->vbo1.block(0);
  b->clearPending();
  CPPUNIT_ASSERT_EQUAL(false, b->needUpdate());
  CPPUNIT_ASSERT_EQUAL(false, b->hasPendingData());
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_end);

  win->vbo1.remove(0);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_end);

  win->vbo1.remove(1);
  CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());
  CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(1_z, b->m_pending_end);
}

//--------------------------------------------------------------------------
void GLBufferTests::testSwap()
{
  // Swap two elements. Check pending data indexes changed.
  auto b = win->vbo1.block(0);
  b->clearPending();
  CPPUNIT_ASSERT_EQUAL(false, b->needUpdate());
  CPPUNIT_ASSERT_EQUAL(false, b->hasPendingData());
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_end);

  bool res = win->vbo1.swap(0, 1);
  CPPUNIT_ASSERT_EQUAL(true, res);
  CPPUNIT_ASSERT_EQUAL(false, b->needUpdate());
  CPPUNIT_ASSERT_EQUAL(false, b->hasPendingData());
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_start);
  CPPUNIT_ASSERT_EQUAL(INITIAL_INDEX, b->m_pending_end);
  // Alternative possible behavior but not choosen
  //CPPUNIT_ASSERT_EQUAL(true, b->needUpdate());
  //CPPUNIT_ASSERT_EQUAL(true, b->hasPendingData());
  //CPPUNIT_ASSERT_EQUAL(0_z, b->m_pending_start);
  //CPPUNIT_ASSERT_EQUAL(1_z, b->m_pending_end);
}
