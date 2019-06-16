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

#include "GLObjectTests.hpp"

class FakeObject : public IGLObject<GLint>
{
public:

  FakeObject()
    : IGLObject<GLint>("FakeObject")
  {
    reset();
  }

  virtual ~FakeObject() { destroy(); }

  void reset()
  {
    IGLObject<GLint>::init();
    m_position = 0;
    m_setup_position = 0;
    m_create_position = 0;
    m_activate_position = 0;
    m_update_position = 0;
    m_deactivate_position = 0;
    m_release_position = 0;
  }

private:

  virtual bool setup() override
  {
    m_setup_position = m_position++;
    return false;
  }

  virtual bool create() override
  {
    m_handle = 0; // fake creation
    m_create_position = m_position++;
    return false;
  }

  virtual void activate() override
  {
    m_activate_position = m_position++;
  }

  virtual bool update() override
  {
    m_update_position = m_position++;
    return false;
  }

  virtual void deactivate() override
  {
    m_deactivate_position = m_position++;
  }

  virtual void release() override
  {
    m_release_position = m_position++;
  }

public:

  uint32_t m_position;
  uint32_t m_setup_position;
  uint32_t m_create_position;
  uint32_t m_activate_position;
  uint32_t m_update_position;
  uint32_t m_deactivate_position;
  uint32_t m_release_position;
};

//--------------------------------------------------------------------------
void GLObjectTests::setUp()
{
  std::cout << "*** GLObjectTests ***************************************" << std::endl;
}

//--------------------------------------------------------------------------
void GLObjectTests::tearDown()
{
}

//--------------------------------------------------------------------------
void GLObjectTests::tests()
{
  FakeObject obj;

  // --- Check init
  CPPUNIT_ASSERT_EQUAL(obj.m_handle, -1);
  CPPUNIT_ASSERT_EQUAL(obj.gpuID(), -1);
  CPPUNIT_ASSERT_EQUAL(obj.m_target, 0U);
  CPPUNIT_ASSERT_EQUAL(obj.target(), 0U);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_setup, true);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_create, true);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_update, false);
  CPPUNIT_ASSERT_EQUAL(obj.m_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_setup_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_create_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_activate_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_update_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_deactivate_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_release_position, 0u);

  // --- glBegin(FakeObject)
  obj.begin();
  CPPUNIT_ASSERT_EQUAL(obj.m_handle, 0);
  CPPUNIT_ASSERT_EQUAL(obj.gpuID(), 0);
  CPPUNIT_ASSERT_EQUAL(obj.m_target, 0U);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_setup, false);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_create, false);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_update, false);
  CPPUNIT_ASSERT_EQUAL(obj.m_position, 3u);
  CPPUNIT_ASSERT_EQUAL(obj.m_create_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_activate_position, 1u);
  CPPUNIT_ASSERT_EQUAL(obj.m_setup_position, 2u);
  CPPUNIT_ASSERT_EQUAL(obj.m_deactivate_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_update_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_release_position, 0u);

  // --- glEnd(FakeObject)
  obj.end();
  CPPUNIT_ASSERT_EQUAL(obj.m_handle, 0);
  CPPUNIT_ASSERT_EQUAL(obj.m_target, 0U);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_setup, false);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_create, false);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_update, false);
  CPPUNIT_ASSERT_EQUAL(obj.m_position, 4u);
  CPPUNIT_ASSERT_EQUAL(obj.m_create_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_activate_position, 1u);
  CPPUNIT_ASSERT_EQUAL(obj.m_setup_position, 2u);
  CPPUNIT_ASSERT_EQUAL(obj.m_deactivate_position, 3u);
  CPPUNIT_ASSERT_EQUAL(obj.m_update_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_release_position, 0u);

  // --- Force update fake object
  obj.forceUpdate();
  CPPUNIT_ASSERT_EQUAL(obj.m_handle, 0);
  CPPUNIT_ASSERT_EQUAL(obj.m_target, 0U);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_setup, false);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_create, false);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_update, true);
  obj.begin();
  CPPUNIT_ASSERT_EQUAL(obj.m_position, 6u);
  CPPUNIT_ASSERT_EQUAL(obj.m_create_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_activate_position, 4u);
  CPPUNIT_ASSERT_EQUAL(obj.m_setup_position, 2u);
  CPPUNIT_ASSERT_EQUAL(obj.m_deactivate_position, 3u);
  CPPUNIT_ASSERT_EQUAL(obj.m_update_position, 5u);
  CPPUNIT_ASSERT_EQUAL(obj.m_release_position, 0u);
  obj.end();
  CPPUNIT_ASSERT_EQUAL(obj.m_position, 7u);
  CPPUNIT_ASSERT_EQUAL(obj.m_create_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_activate_position, 4u);
  CPPUNIT_ASSERT_EQUAL(obj.m_setup_position, 2u);
  CPPUNIT_ASSERT_EQUAL(obj.m_deactivate_position, 6u);
  CPPUNIT_ASSERT_EQUAL(obj.m_update_position, 5u);
  CPPUNIT_ASSERT_EQUAL(obj.m_release_position, 0u);

  // --- Destroy object
  obj.destroy();
  CPPUNIT_ASSERT_EQUAL(obj.m_handle, -1);
  CPPUNIT_ASSERT_EQUAL(obj.m_target, 0U);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_setup, true);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_create, true);
  CPPUNIT_ASSERT_EQUAL(obj.m_need_update, false);
  CPPUNIT_ASSERT_EQUAL(obj.m_position, 7u);
  CPPUNIT_ASSERT_EQUAL(obj.m_create_position, 0u);
  CPPUNIT_ASSERT_EQUAL(obj.m_activate_position, 4u);
  CPPUNIT_ASSERT_EQUAL(obj.m_setup_position, 2u);
  CPPUNIT_ASSERT_EQUAL(obj.m_deactivate_position, 6u);
  CPPUNIT_ASSERT_EQUAL(obj.m_update_position, 5u);
  CPPUNIT_ASSERT_EQUAL(obj.m_release_position, 0u);
}
