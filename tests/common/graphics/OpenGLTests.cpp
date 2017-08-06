//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "OpenGLTests.hpp"
#include "GLWindow.hpp"

// This class is made for creating an OpenGL context
class GLWindow1: public IGLWindow
{
public:

  GLWindow1()
    : IGLWindow(),
      vao("VAO"),
      attrib("attrib")
  {
    std::cout << "GLWindow(AA)" << std::endl;
  }

  ~GLWindow1()
  {
    std::cout << "~GLWindow(AA)" << std::endl;
  }

  virtual bool setup() override
  {
    // Check dummy VAO
    CPPUNIT_ASSERT_EQUAL(true, vao.m_need_setup);
    CPPUNIT_ASSERT_EQUAL(true, vao.m_need_update);
    CPPUNIT_ASSERT_EQUAL(true, vao.m_need_create);
    CPPUNIT_ASSERT_EQUAL(true, vao.m_throw_enable);
    CPPUNIT_ASSERT_EQUAL(true, vao.needCreate());
    CPPUNIT_ASSERT_EQUAL(true, vao.needSetup());
    CPPUNIT_ASSERT_EQUAL(true, vao.needUpdate());
    CPPUNIT_ASSERT_EQUAL(false, vao.canBeReleased());
    CPPUNIT_ASSERT_EQUAL(0U, vao.m_handle);
    CPPUNIT_ASSERT_EQUAL(true, 0U == vao);
    CPPUNIT_ASSERT_EQUAL(0U, vao.m_target);
    CPPUNIT_ASSERT_EQUAL(0, vao.m_name.compare("VAO"));
    CPPUNIT_ASSERT_EQUAL(false, vao.isValid());

    // Check dummy attrib
    CPPUNIT_ASSERT_EQUAL(true, attrib.m_need_setup);
    CPPUNIT_ASSERT_EQUAL(true, attrib.m_need_update);
    CPPUNIT_ASSERT_EQUAL(true, attrib.m_need_create);
    CPPUNIT_ASSERT_EQUAL(true, attrib.m_throw_enable);
    CPPUNIT_ASSERT_EQUAL(true, attrib.needCreate());
    CPPUNIT_ASSERT_EQUAL(true, attrib.needSetup());
    CPPUNIT_ASSERT_EQUAL(true, attrib.needUpdate());
    CPPUNIT_ASSERT_EQUAL(false, attrib.canBeReleased());
    CPPUNIT_ASSERT_EQUAL((GLenum) -1, attrib.m_handle);
    CPPUNIT_ASSERT_EQUAL(true, -1 == attrib);
    CPPUNIT_ASSERT_EQUAL(0U, attrib.m_target);
    CPPUNIT_ASSERT_EQUAL(0, attrib.m_name.compare("attrib"));
    CPPUNIT_ASSERT_EQUAL(false, attrib.isValid());

    // Try destroy GL object not created. Check function was not called.
    vao.m_need_setup = false;
    vao.m_need_update = false;
    vao.m_need_create = false;
    vao.m_throw_enable = false;
    vao.destroy();
    CPPUNIT_ASSERT_EQUAL(false, vao.m_need_setup);
    CPPUNIT_ASSERT_EQUAL(false, vao.m_need_update);
    CPPUNIT_ASSERT_EQUAL(false, vao.m_need_create);
    CPPUNIT_ASSERT_EQUAL(false, vao.m_throw_enable);
    CPPUNIT_ASSERT_EQUAL(false, vao.needCreate());
    CPPUNIT_ASSERT_EQUAL(false, vao.needSetup());
    CPPUNIT_ASSERT_EQUAL(false, vao.needUpdate());
    CPPUNIT_ASSERT_EQUAL(true, vao.canBeReleased());
    CPPUNIT_ASSERT_EQUAL(0U, vao.m_handle);
    CPPUNIT_ASSERT_EQUAL(true, 0 == vao);
    CPPUNIT_ASSERT_EQUAL(0U, vao.m_target);
    CPPUNIT_ASSERT_EQUAL(0, vao.m_name.compare("VAO"));
    CPPUNIT_ASSERT_EQUAL(false, vao.isValid());
    vao.m_need_setup = true;
    vao.m_need_update = true;
    vao.m_need_create = true;
    vao.m_throw_enable = true;

    // Start VAO
    vao.start();
    CPPUNIT_ASSERT_EQUAL(false, vao.m_need_setup);
    CPPUNIT_ASSERT_EQUAL(false, vao.m_need_update);
    CPPUNIT_ASSERT_EQUAL(false, vao.m_need_create);
    CPPUNIT_ASSERT_EQUAL(true, vao.m_throw_enable);
    CPPUNIT_ASSERT_EQUAL(false, vao.needCreate());
    CPPUNIT_ASSERT_EQUAL(false, vao.needSetup());
    CPPUNIT_ASSERT_EQUAL(false, vao.needUpdate());
    CPPUNIT_ASSERT_EQUAL(true, vao.canBeReleased());
    CPPUNIT_ASSERT_EQUAL(1U, vao.m_handle);
    CPPUNIT_ASSERT_EQUAL(true, 1U == vao);
    CPPUNIT_ASSERT_EQUAL(0U, vao.m_target);
    CPPUNIT_ASSERT_EQUAL(0, vao.m_name.compare("VAO"));
    CPPUNIT_ASSERT_EQUAL(true, vao.isValid());
    vao.stop();

    // Start attrib
    CPPUNIT_ASSERT_THROW(attrib.start(), GLObjectExceptionNoShader);
    CPPUNIT_ASSERT_EQUAL(true, attrib.m_need_setup);
    CPPUNIT_ASSERT_EQUAL(true, attrib.m_need_update);
    CPPUNIT_ASSERT_EQUAL(false, attrib.m_need_create);
    CPPUNIT_ASSERT_EQUAL(true, attrib.m_throw_enable);
    CPPUNIT_ASSERT_EQUAL(false, attrib.needCreate());
    CPPUNIT_ASSERT_EQUAL(true, attrib.needSetup());
    CPPUNIT_ASSERT_EQUAL(true, attrib.needUpdate());
    CPPUNIT_ASSERT_EQUAL(true, attrib.canBeReleased());
    CPPUNIT_ASSERT_EQUAL((GLenum) -1, attrib.m_handle);
    CPPUNIT_ASSERT_EQUAL(true, -1 == attrib);
    CPPUNIT_ASSERT_EQUAL(0U, attrib.m_target);
    CPPUNIT_ASSERT_EQUAL(0, attrib.m_name.compare("attrib"));
    CPPUNIT_ASSERT_EQUAL(false, attrib.isValid());

#warning "A finir"
    // Start a program shader
    // shader.load();
    //attrib.setup();
    //CPPUNIT_ASSERT_EQUAL();
    //attrib.start()
    //CPPUNIT_ASSERT_EQUAL();
    attrib.stop();

    return true;
  }
  virtual bool draw() override
  {
    // Do not draw
    return false;
  }

  GLVertexArray vao;
  GLAttrib attrib;
};

static std::shared_ptr<GLWindow1> win;

//--------------------------------------------------------------------------
void OpenGLTests::setUp()
{
  win = std::make_shared<GLWindow1>();
}

//--------------------------------------------------------------------------
void OpenGLTests::tearDown()
{
  win.reset();
}

//--------------------------------------------------------------------------
void OpenGLTests::testOpenGL()
{
  CPPUNIT_ASSERT_EQUAL(0, win->create());
}
